/**
 * File name: whalesynth_lv2_dsp.cpp
 * Project: WhaleSynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of WhaleSynth.
 *
 * WhaleSynth is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "Synthesizer.h"
#include "Note.h"
#include "UridMap.h"
#include "SynthesizerProxyLv2.h"

#include <vector>
#include <memory>
#include <atomic>

class WhaleSynthLv2DSPPlugin
{
  public:
        using CommandType = SynthesizerProxyLv2::CommandType;
        WhaleSynthLv2DSPPlugin()
                : whalesynth{new Synthesizer}
                , midiInPort{nullptr}
                , eventsInPort{nullptr}
                , eventsOutPort{nullptr}
                , notifyHostPort{nullptr}
        {
        }

        ~WhaleSynthLv2DSPPlugin()
        {
                if (whalesynth)
                        delete whalesynth;
        }

        void setUriIdMap(const UridIdMap &map)
        {
                uridIdMap = map;
        }

        const UridIdMap& getUridIdMap() const
        {
                return uridIdMap;
        }

        size_t numberOfChannels() const
        {
                return whalesynth->numberOfChannels();
        }

        void setAudioChannelPort(float *data, size_t channel)
        {
                if (channel < 2 * WhaleSynth::defaultChannelsNumber)
                        outputChannels[channel] = data;
        }

        void setMidiInPort(LV2_Atom_Sequence *data)
        {
                midiInPort = data;
        }

        void setNotifyHostPort(LV2_Atom_Sequence *data)
        {
                notifyHostPort = data;
        }

        void setInEventsPort(LV2_Atom_Sequence *data)
        {
                eventsInPort = data;
        }

        void setOutEventsPort(LV2_Atom_Sequence *data)
        {
                eventsOutPort = data;
        }

        void setStateData(const std::string &data, int flags = 0)
        {
                WHALE_UNUSED(flags);
                WHALE_UNUSED(data);
        }

        std::string getStateData()
        {
                return "";//whalesynth->getKitState()->toJson();
        }

        Synthesizer* getSynthesizer() const
        {
                return whalesynth;
        }

        bool isNote(const uint8_t* buffer) const
        {
                return (((*(buffer) & 0xf0)) == 0x90) || (((*(buffer) & 0xf0)) == 0x80);
        }

        Note getNote(const uint8_t* buffer) const
        {
                Note note = {WhaleSynth::NoMIDIKey,
                             WhaleSynth::MaxMIDIKeyVelocity,
                             MIDIKeyState::MIDIKeyStateOff};

                if (((*(buffer) & 0xf0)) == 0x90) {
                        note.midiKeyState    = MIDIKeyState::MIDIKeyStateOn;
                        note.midiKeyId       = buffer[1];
                        note.midiKeyVelocity = buffer[2];
                } else if(((*(buffer) & 0xf0)) == 0x80) {
                        note.midiKeyId       = buffer[1];
                        note.midiKeyVelocity = buffer[2];
                }
                return note;
        }

        void processSamples(int nsamples)
        {
                if (!midiInPort)
                        return;

                auto it = lv2_atom_sequence_begin(&eventsInPort->body);
                while (!lv2_atom_sequence_is_end(&eventsInPort->body, eventsInPort->atom.size, it)) {
                        auto obj = (LV2_Atom_Object*)(&it->body);
                        const LV2_Atom* command  = NULL;
                        lv2_atom_object_get(obj, uridIdMap.command, &command, 0);
                        if (command) {
                                WHALE_LOG_DEBUG("new command: ");
                                auto commandType = reinterpret_cast<const LV2_Atom_Int*>(command)->body;
                                if (CommandType::SetWaveFunction == static_cast<CommandType>(commandType)) {
                                        WHALE_LOG_DEBUG("set wave function");
                                        const LV2_Atom* operatorId  = NULL;
                                        const LV2_Atom* waveType    = NULL;
                                        lv2_atom_object_get(obj, uridIdMap.id, &operatorId,
                                                            uridIdMap.waveType, &waveType, 0);
                                        if (operatorId && waveType) {
                                                auto id   = reinterpret_cast<const LV2_Atom_Int*>(operatorId)->body;
                                                auto wave = reinterpret_cast<const LV2_Atom_Int*>(waveType)->body;
                                                whalesynth->setWave(id, static_cast<WaveGenerator::WaveFunctionType>(wave));
                                        }
                                }
                        }

                        it = lv2_atom_sequence_next(it);
                }

                it = lv2_atom_sequence_begin(&midiInPort->body);
                size_t currentFrame = 0;
                while (!lv2_atom_sequence_is_end(&midiInPort->body, midiInPort->atom.size, it)) {
                        auto eventFrame = it->time.frames;
                        auto size = eventFrame - currentFrame;
                        if (size > 0)
                                whalesynth->process(outputChannels, size);

                        const uint8_t* const msg = (const uint8_t*)(it + 1);
                        if (isNote(msg))
                                whalesynth->setNote(getNote(msg));
                        currentFrame = eventFrame;
                        it = lv2_atom_sequence_next(it);
                }

                if (currentFrame < static_cast<decltype(currentFrame)>(nsamples))
                        whalesynth->process(outputChannels, nsamples - currentFrame);
        }

        void notifyHost() const
        {
                /*                if (!notifyHostChannel)
                        return;

                auto sequence = static_cast<LV2_Atom_Sequence*>(notifyHostChannel);
                size_t neededAtomSize = sizeof(LV2_Atom_Event) + sizeof(LV2_Atom_Object_Body);
                if (sequence) {
                        sequence->atom.type = getAtomSequence();
                        sequence->atom.size = sizeof(LV2_Atom_Sequence_Body);
                        sequence->body.unit = 0;
                        sequence->body.pad  = 0;
                        auto event = reinterpret_cast<LV2_Atom_Event*>(sequence + 1);
                        event->time.frames = 0;
                        auto atomObject = reinterpret_cast<LV2_Atom_Object*>(&event->body);
                        atomObject->atom.type = getAtomObject();
                        atomObject->atom.size = sizeof(LV2_Atom_Object_Body);
                        atomObject->body.id = 0;
                        atomObject->body.otype = getAtomStateChanged();
                        sequence->atom.size += neededAtomSize;
                }
                */
        }

protected:


private:
        Synthesizer *whalesynth;
        LV2_Atom_Sequence *midiInPort;
        LV2_Atom_Sequence *eventsInPort;
        LV2_Atom_Sequence *eventsOutPort;
        LV2_Atom_Sequence *notifyHostPort;
        float* outputChannels[WhaleSynth::defaultChannelsNumber * 2];
        UridIdMap uridIdMap;
};

static LV2_Handle whale_instantiate(const LV2_Descriptor*     descriptor,
                                    double                    rate,
                                    const char*               bundle_path,
                                    const LV2_Feature* const* features)
{
        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_URID__map)) {
                        auto uridMap = static_cast<LV2_URID_Map*>(feature->data);
                        if (!uridMap || !uridMap->map || !uridMap->handle) {
                                WHALE_LOG_ERROR("can't get urid map");
                                return nullptr;
                        } else {
                                auto whaleSynthLv2PLugin = new WhaleSynthLv2DSPPlugin;
                                whaleSynthLv2PLugin->setUriIdMap(createUriIdMap(uridMap));
                                return static_cast<LV2_Handle>(whaleSynthLv2PLugin);
                        }
                        break;
                }
                features++;
        }
        return nullptr;
}

static void whale_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        auto whalesynthLv2PLugin = static_cast<WhaleSynthLv2DSPPlugin*>(instance);
	size_t portNumber = static_cast<size_t>(port);
        if (portNumber == 0 || portNumber == 1)
                whalesynthLv2PLugin->setAudioChannelPort(static_cast<float*>(data), portNumber);
        else if (portNumber == 2)
                whalesynthLv2PLugin->setMidiInPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 3)
                whalesynthLv2PLugin->setInEventsPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 4)
                whalesynthLv2PLugin->setOutEventsPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 5)
                whalesynthLv2PLugin->setNotifyHostPort(static_cast<LV2_Atom_Sequence*>(data));
}

static void whale_activate(LV2_Handle instance)
{
}

static void whale_run(LV2_Handle instance, uint32_t n_samples)
{
       auto plugin = static_cast<WhaleSynthLv2DSPPlugin*>(instance);
       plugin->processSamples(n_samples);
}

static void whale_deactivate(LV2_Handle instance)
{
}

static void whale_cleaup(LV2_Handle instance)
{
        delete static_cast<WhaleSynthLv2DSPPlugin*>(instance);
}

static LV2_State_Status
whale_state_save(LV2_Handle                instance,
                 LV2_State_Store_Function  store,
                 LV2_State_Handle          handle,
                 uint32_t                  flags,
                 const LV2_Feature* const* features)
{
        /*        auto whalesynthLv2PLugin = static_cast<WhaleSynthLv2DSPPlugin*>(instance);
        if (whalesynthLv2PLugin){
                std::string stateData = whalesynthLv2PLugin->getStateData();
                store(handle, whalesynthLv2PLugin->getStateId(), stateData.data(),
                      stateData.size(), whalesynthLv2PLugin->getAtomChunkId(),
                      LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE);
        }
        */
        return LV2_STATE_SUCCESS;
}

static LV2_State_Status
whale_state_restore(LV2_Handle                  instance,
                    LV2_State_Retrieve_Function retrieve,
                    LV2_State_Handle            handle,
                    uint32_t                    flags,
                    const LV2_Feature* const*   features)
{
        /*auto whalesynthLv2PLugin = static_cast<WhaleSynthLv2DSPPlugin*>(instance);
        if (whalesynthLv2PLugin) {
                size_t size   = 0;
                LV2_URID type = 0;
                const char *data = (const char*)retrieve(handle, whalesynthLv2PLugin->getStateId(),
                                                         &size, &type, &flags);
                if (data && size > 0)
                        whalesynthLv2PLugin->setStateData(std::string(data, size), flags);
                        }*/
        return LV2_STATE_SUCCESS;
}

static const void* whale_extention_data(const char* uri)
{
        static const LV2_State_Interface state = {whale_state_save, whale_state_restore};
        if (std::string(uri) == std::string(LV2_STATE__interface))
                return &state;
        return nullptr;
}

static const LV2_Descriptor whale_descriptor = {
	WHALESYNTH_URI,
	whale_instantiate,
	whale_connect_port,
	whale_activate,
	whale_run,
	whale_deactivate,
	whale_cleaup,
	whale_extention_data,
};

const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:  return &whale_descriptor;
	default: return nullptr;
	}
}
