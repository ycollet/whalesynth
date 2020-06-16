/**
 * File name: geonsynth_lv2.cpp
 * Project: Geonsynth (A software synthesizer)
 *
 * Copyright (C) 2020Iurie Nistor <http://iuriepage.wordpress.com>
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
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>
#include <lv2/lv2plug.in/ns/ext/instance-access/instance-access.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>

//#include "mainwindow.h"
#include "Synthesizer.h"
#include "Note.h"
//#include "kit_state.h"

//#include <RkMain.h>
//#include <RkPlatform.h>

#include <vector>
#include <memory>
#include <atomic>

#define GEONSYNTH_URI "http://iuriepage.wordpress.com/geonsynth"
#define GEONSYNTH_URI_UI "http://iuriepage.wordpress.com/geonsynth#ui"
#define GEONSYNTH_URI_STATE "http://iuriepage.wordpress.com/geonsynth#state"
#define GEONSYNTH_URI_STATE_CHANGED "http://lv2plug.in/ns/ext/state#StateChanged"

class GeonsynthLv2Plugin
{
  public:
        GeonsynthLv2Plugin()
                : geonSynth{new Synthesizer}
                , midiIn{nullptr}
                , notifyHostChannel{nullptr}
                , atomInfo{0}
        {
        }

        ~GeonsynthLv2Plugin()
        {
                if (geonSynth)
                        delete geonSynth;
        }

        size_t numberOfChannels() const
        {
                return geonSynth->numberOfChannels();
        }

        void setAudioChannel(float *data, size_t channel)
        {
                if (channel < 2 * GeonSynth::defaultChannelsNumber)
                        outputChannels[channel] = data;
        }

        void setMidiIn(LV2_Atom_Sequence *data)
        {
                midiIn = data;
        }

        void setNotifyHostChannel(LV2_Atom_Sequence *data)
        {
                notifyHostChannel = data;
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
                if (!midiIn)
                        return;
                auto it = lv2_atom_sequence_begin(&midiIn->body);
                size_t currentFrame = 0;
                while (!lv2_atom_sequence_is_end(&midiIn->body, midiIn->atom.size, it)) {
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
        LV2_Atom_Sequence *midiIn;
        LV2_Atom_Sequence *notifyHostChannel;
        float* outputChannels[GeonSynth::defaultChannelsNumber * 2];

        struct AtomInfo {
                LV2_URID stateId;
                LV2_URID atomStringId;
                LV2_URID atomSequence;
                LV2_URID atomStateChanged;
                LV2_URID atomObject;
        };

        AtomInfo atomInfo;
        //        std::atomic<bool> kickIsUpdated;
};

/**
 * Creates and shows an instance of Geonsynth GUI that takes
 * the geonsynth API instance as a pointer.
 */
static LV2UI_Handle gsynth_instantiate_ui(const LV2UI_Descriptor*   descriptor,
                                         const char*               plugin_uri,
                                         const char*               bundle_path,
                                         LV2UI_Write_Function      write_function,
                                         LV2UI_Controller          controller,
                                         LV2UI_Widget*             widget,
                                         const LV2_Feature* const* features)
{
        /*GeonsynthLv2Plugin *geonsynthLv2PLugin = nullptr;
        if (!features) {
                return nullptr;
        }

        void *parent = nullptr;
        LV2UI_Resize *resize = nullptr;
        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_UI__parent))
                        parent = feature->data;

                if (std::string(feature->URI) == std::string(LV2_UI__resize))
                        resize = (LV2UI_Resize*)feature->data;

                if (std::string(feature->URI) == std::string(LV2_INSTANCE_ACCESS_URI)) {
                        geonsynthLv2PLugin = static_cast<GeonsynthLv2Plugin*>(feature->data);
                        if (!geonsynthLv2PLugin)
                                return nullptr;
                }
                features++;
        }

        // Get the info about X Window parent window.
        const uintptr_t parentWinId = (uintptr_t)parent;
        Display* xDisplay = XOpenDisplay(nullptr);
        int screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, parentWinId);

        auto guiApp = new RkMain();
        geonsynthLv2PLugin->getApi()->setEventQueue(guiApp->eventQueue());
        auto mainWidget = new MainWindow(guiApp, geonsynthLv2PLugin->getApi(), info);
        if (!mainWidget->init()) {
                GEONSYNTH_LOG_ERROR("can't init main window");
                delete guiApp;
                return nullptr;
        }

        auto winId = mainWidget->nativeWindowInfo()->window;
        *widget = (LV2UI_Widget)static_cast<uintptr_t>(winId);
        auto size = mainWidget->size();
        resize->ui_resize(resize->handle, size.width(), size.height());
        return static_cast<LV2UI_Handle>(guiApp);*/
        return nullptr;
}

static void gsynth_cleanup_ui(LV2UI_Handle handle)
{
        // if (handle)
        //         delete static_cast<RkMain*>(handle);
}

static void gsynth_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer )
{
}

// static int gsynth_idle(LV2UI_Handle ui)
// {
//         GSYNTH_UNUSED(ui);
//         return 0;
// }

static const void* gsynth_extension_data(const char* uri)
{
    // static const LV2UI_Idle_Interface idleInterface = {gsynth_idle};
    // if (std::string(uri) == std::string(LV2_UI__idleInterface))
    //         return &idleInterface;
    return nullptr;
}

static const LV2UI_Descriptor gsynth_descriptor_ui = {
	GEONSYNTH_URI_UI,
	gsynth_instantiate_ui,
	gsynth_cleanup_ui,
	gsynth_port_event_ui,
	gsynth_extension_data
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:	return &gsynth_descriptor_ui;
	default: return NULL;
        }
}

static LV2_Handle gsynth_instantiate(const LV2_Descriptor*     descriptor,
                                    double                    rate,
                                    const char*               bundle_path,
                                    const LV2_Feature* const* features)
{
        auto geonsynthLv2PLugin = new GeonsynthLv2Plugin;

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
        auto geonsynthLv2PLugin = static_cast<GeonsynthLv2Plugin*>(instance);
	auto nChannels = geonsynthLv2PLugin->numberOfChannels();
	auto portNumber = static_cast<decltype(nChannels)>(port);
        if (portNumber == 0)
                geonsynthLv2PLugin->setMidiIn(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber == 1)
                geonsynthLv2PLugin->setNotifyHostChannel(static_cast<LV2_Atom_Sequence*>(data));
        else if (portNumber > 2)
                geonsynthLv2PLugin->setAudioChannel(static_cast<float*>(data), portNumber - 3);
}

static void gsynth_activate(LV2_Handle instance)
{
}

static void gsynth_run(LV2_Handle instance, uint32_t n_samples)
{
       auto geonsynthLv2PLugin = static_cast<GeonsynthLv2Plugin*>(instance);
       geonsynthLv2PLugin->processSamples(n_samples);
}

static void gsynth_deactivate(LV2_Handle instance)
{
}

static void gsynth_cleaup(LV2_Handle instance)
{
        delete static_cast<GeonsynthLv2Plugin*>(instance);
}

static LV2_State_Status
gsynth_state_save(LV2_Handle                instance,
                 LV2_State_Store_Function  store,
                 LV2_State_Handle          handle,
                 uint32_t                  flags,
                 const LV2_Feature* const* features)
{
        /*        auto geonsynthLv2PLugin = static_cast<GeonsynthLv2Plugin*>(instance);
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
        /*auto geonsynthLv2PLugin = static_cast<GeonsynthLv2Plugin*>(instance);
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
