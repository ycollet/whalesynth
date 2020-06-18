/**
 * File name: geonsynth_lv2_dsp.cpp
 * Project: Geonsynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Geonsynth.
 *
 * Geonsynth is free software; you can redistribute it and/or modify
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

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>

#include "Synthesizer.h"
#include "Note.h"

#include <vector>
#include <memory>
#include <atomic>

#define GEONSYNTH_URI "http://iuriepage.wordpress.com/geonsynth"
#define GEONSYNTH_URI_STATE "http://iuriepage.wordpress.com/geonsynth#state"
#define GEONSYNTH_URI_STATE_CHANGED "http://lv2plug.in/ns/ext/state#StateChanged"

class GeonsynthLv2DSPPlugin
{
  public:
        GeonsynthLv2DSPPlugin()
                : geonSynth{new Synthesizer}
                , midiInPort{nullptr}
                , eventsInPort{nullptr}
                , eventsOutPort{nullptr}
                , notifyHostPort{nullptr}
                , atomInfo{0}
        {
        }

        ~GeonsynthLv2DSPPlugin()
        {
                if (geonSynth)
                        delete geonSynth;
        }

        size_t numberOfChannels() const
        {
                return geonSynth->numberOfChannels();
        }

        void setAudioChannelPort(float *data, size_t channel)
        {
                if (channel < 2 * GeonSynth::defaultChannelsNumber)
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

        void setStateId(LV2_URID id)
        {
                atomInfo.stateId = id;
        }

        LV2_URID getStateId() const
        {
                return atomInfo.stateId;
        }

        void setAtomChunkId(LV2_URID id)
        {
                atomInfo.atomStringId = id;
        }

        LV2_URID getAtomChunkId() const
        {
                return atomInfo.atomStringId;
        }

        void setAtomSequence(LV2_URID sequence)
        {
                atomInfo.atomSequence = sequence;
        }

        LV2_URID getAtomSequence(void) const
        {
                return atomInfo.atomSequence;
        }

        void setAtomStateChanged(LV2_URID changed)
        {
                atomInfo.atomStateChanged = changed;
        }

        LV2_URID getAtomStateChanged(void) const
        {
                return atomInfo.atomStateChanged;
        }

        void setAtomObject(LV2_URID object)
        {
                atomInfo.atomObject = object;
        }

        LV2_URID getAtomObject(void) const
        {
                return atomInfo.atomObject;
        }

        void setStateData(const std::string &data, int flags = 0)
        {
                GSYNTH_UNUSED(flags);
                GSYNTH_UNUSED(data);
        }

        std::string getStateData()
        {
                return "";//geonSynth->getKitState()->toJson();
        }

        Synthesizer* getSynthesizer() const
        {
                return geonSynth;
        }

        bool isNote(const uint8_t* buffer) const
        {
                return (((*(buffer) & 0xf0)) == 0x90) || (((*(buffer) & 0xf0)) == 0x80);
        }

        Note getNote(const uint8_t* buffer) const
        {
                Note note = {GeonSynth::NoMIDIKey,
                             GeonSynth::MaxMIDIKeyVelocity,
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
                        LV2_Atom_Int* v = (LV2_Atom_Int*)(&it->body);
                        int *vv = (int*)LV2_ATOM_BODY(v);
                        GSYNTH_LOG_DEBUG("IN: body :" << *vv);
                        geonSynth->setWave(static_cast<WaveGenerator::WaveFunctionType>(*vv));
                        it = lv2_atom_sequence_next(it);
                }

                it = lv2_atom_sequence_begin(&midiInPort->body);
                size_t currentFrame = 0;
                while (!lv2_atom_sequence_is_end(&midiInPort->body, midiInPort->atom.size, it)) {
                        auto eventFrame = it->time.frames;
                        auto size = eventFrame - currentFrame;
                        if (size > 0)
                                geonSynth->process(outputChannels, size);

                        const uint8_t* const msg = (const uint8_t*)(it + 1);
                        if (isNote(msg))
                                geonSynth->setNote(getNote(msg));
                        currentFrame = eventFrame;
                        it = lv2_atom_sequence_next(it);
                }

                if (currentFrame < static_cast<decltype(currentFrame)>(nsamples))
                        geonSynth->process(outputChannels, nsamples - currentFrame);
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
        Synthesizer *geonSynth;
        LV2_Atom_Sequence *midiInPort;
        LV2_Atom_Sequence *eventsInPort;
        LV2_Atom_Sequence *eventsOutPort;
        LV2_Atom_Sequence *notifyHostPort;
        float* outputChannels[GeonSynth::defaultChannelsNumber * 2];

        struct AtomInfo {
                LV2_URID stateId;
                LV2_URID atomStringId;
                LV2_URID atomSequence;
                LV2_URID atomStateChanged;
                LV2_URID atomObject;
        };

        AtomInfo atomInfo;
};

static LV2_Handle gsynth_instantiate(const LV2_Descriptor*     descriptor,
                                    double                    rate,
                                    const char*               bundle_path,
                                    const LV2_Feature* const* features)
{
        auto geonsynthLv2PLugin = new GeonsynthLv2DSPPlugin;

        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_URID__map)) {
                        auto uridMap = static_cast<LV2_URID_Map*>(feature->data);
                        if (uridMap && uridMap->map && uridMap->handle) {
                                geonsynthLv2PLugin->setStateId(uridMap->map(uridMap->handle, GEONSYNTH_URI_STATE));
                                geonsynthLv2PLugin->setAtomChunkId(uridMap->map(uridMap->handle, LV2_ATOM__Chunk));
                                geonsynthLv2PLugin->setAtomSequence(uridMap->map(uridMap->handle, LV2_ATOM__Sequence));
                                geonsynthLv2PLugin->setAtomStateChanged(uridMap->map(uridMap->handle, GEONSYNTH_URI_STATE_CHANGED));
                                geonsynthLv2PLugin->setAtomObject(uridMap->map(uridMap->handle, LV2_ATOM__Object));
                                GSYNTH_LOG_INFO("IDDDDD: " << uridMap->map(uridMap->handle, LV2_ATOM__Object));
                                GSYNTH_LOG_INFO("LV2_ATOM__FloatID: " << uridMap->map(uridMap->handle, LV2_ATOM__Float));
                        }
                        break;
                }
                features++;
        }

        return static_cast<LV2_Handle>(geonsynthLv2PLugin);
}

static void gsynth_connect_port(LV2_Handle instance,
                               uint32_t   port,
                               void*      data)
{
        auto geonsynthLv2PLugin = static_cast<GeonsynthLv2DSPPlugin*>(instance);
	size_t portNumber = static_cast<size_t>(port);
        if (portNumber == 0 || portNumber == 1)
                geonsynthLv2PLugin->setAudioChannelPort(static_cast<float*>(data), portNumber);
        else if (portNumber == 2)
                geonsynthLv2PLugin->setMidiInPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 3)
                geonsynthLv2PLugin->setInEventsPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 4)
                geonsynthLv2PLugin->setOutEventsPort(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 5)
                geonsynthLv2PLugin->setNotifyHostPort(static_cast<LV2_Atom_Sequence*>(data));
}

static void gsynth_activate(LV2_Handle instance)
{
}

static void gsynth_run(LV2_Handle instance, uint32_t n_samples)
{
       auto plugin = static_cast<GeonsynthLv2DSPPlugin*>(instance);
       plugin->processSamples(n_samples);
}

static void gsynth_deactivate(LV2_Handle instance)
{
}

static void gsynth_cleaup(LV2_Handle instance)
{
        delete static_cast<GeonsynthLv2DSPPlugin*>(instance);
}

static LV2_State_Status
gsynth_state_save(LV2_Handle                instance,
                 LV2_State_Store_Function  store,
                 LV2_State_Handle          handle,
                 uint32_t                  flags,
                 const LV2_Feature* const* features)
{
        /*        auto geonsynthLv2PLugin = static_cast<GeonsynthLv2DSPPlugin*>(instance);
        if (geonsynthLv2PLugin){
                std::string stateData = geonsynthLv2PLugin->getStateData();
                store(handle, geonsynthLv2PLugin->getStateId(), stateData.data(),
                      stateData.size(), geonsynthLv2PLugin->getAtomChunkId(),
                      LV2_STATE_IS_POD | LV2_STATE_IS_PORTABLE);
        }
        */
        return LV2_STATE_SUCCESS;
}

static LV2_State_Status
gsynth_state_restore(LV2_Handle                  instance,
                    LV2_State_Retrieve_Function retrieve,
                    LV2_State_Handle            handle,
                    uint32_t                    flags,
                    const LV2_Feature* const*   features)
{
        /*auto geonsynthLv2PLugin = static_cast<GeonsynthLv2DSPPlugin*>(instance);
        if (geonsynthLv2PLugin) {
                size_t size   = 0;
                LV2_URID type = 0;
                const char *data = (const char*)retrieve(handle, geonsynthLv2PLugin->getStateId(),
                                                         &size, &type, &flags);
                if (data && size > 0)
                        geonsynthLv2PLugin->setStateData(std::string(data, size), flags);
                        }*/
        return LV2_STATE_SUCCESS;
}

static const void* gsynth_extention_data(const char* uri)
{
        static const LV2_State_Interface state = {gsynth_state_save, gsynth_state_restore};
        if (std::string(uri) == std::string(LV2_STATE__interface))
                return &state;
        return nullptr;
}

static const LV2_Descriptor gsynth_descriptor = {
	GEONSYNTH_URI,
	gsynth_instantiate,
	gsynth_connect_port,
	gsynth_activate,
	gsynth_run,
	gsynth_deactivate,
	gsynth_cleaup,
	gsynth_extention_data,
};

const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:  return &gsynth_descriptor;
	default: return nullptr;
	}
}
