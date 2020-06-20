/**
 * File name: Jack.cpp
 * Project: Whalesynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Whalesynth.
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

#include "Jack.h"
#include "Synthesizer.h"

#include <string.h>

Jack::Jack(Synthesizer &synthesizer)
        : whaleSynth{synthesizer}
        , jackClient{nullptr}
        , midiInPort{nullptr}
        , jackCreated{false}
{
        createJackClient();
}

Jack::~Jack()
{
        if (jackClient) {
                jack_deactivate(jackClient);
                for (auto &ch : outputChannels) {
                        jack_port_unregister(jackClient, ch.first);
                        jack_port_unregister(jackClient, ch.second);
                }
                jack_client_close(jackClient);
        }
}

bool Jack::createJackClient()
{
        jackClient = jack_client_open(WHALESYNTH_NAME, JackNoStartServer, NULL);
        if (jackClient == nullptr) {
                WHALE_LOG_ERROR("can't create jack client");
                return false;
        }

        jack_set_process_callback(jackClient,
                                  jackProcessCallback,
                                  this);
        createMidiInPot();
        createOutputPorts();
        jackCreated = true;
        return jackCreated;
}

void Jack::createMidiInPot()
{
        midiInPort = jack_port_register(jackClient, "MidiIn",
                                        JACK_DEFAULT_MIDI_TYPE,
                                        JackPortIsInput, 0);
        if (midiInPort == NULL) {
                WHALE_LOG_ERROR("can't create MidiIn port");
        }
}

void Jack::createOutputPorts()
{
        for (decltype(channels()) i = 0; i < channels(); i++) {
                auto name = "AudioOut" + ((channels() > 1) ? "-" + std::to_string(i) : "");
                auto portL = jack_port_register(jackClient, (name + "_L").c_str(),
                                                JACK_DEFAULT_AUDIO_TYPE,
                                                JackPortIsOutput, 0);
                auto portR = jack_port_register(jackClient, (name + "_R").c_str(),
                                                JACK_DEFAULT_AUDIO_TYPE,
                                                JackPortIsOutput, 0);
                if (!portL || !portR) {
                        WHALE_LOG_ERROR("can't create jack audio output port " << name);
                } else {
                        outputChannels.push_back({portL, portR});
                }
        }
}

bool Jack::start()
{
        if (!jackClient) {
                WHALE_LOG_ERROR("Jack client was not created");
                return false;
        }

        if (jack_activate(jackClient) != 0) {
                WHALE_LOG_ERROR("can't active Jack client");
                return false;
        } else {
                WHALE_LOG_DEBUG("Jack client activated");
        }

        return true;
}

void Jack::stop()
{
}

bool Jack::isActive() const
{
        return jackCreated;
}

size_t Jack::channels() const
{
        return Whalesynth::defaultChannelsNumber;
}

unsigned int Jack::sampleRate() const
{
        return Whalesynth::defaultSampleRate;
}

bool Jack::isNote(const jack_midi_event_t *event) const
{
        return (((*(event->buffer) & 0xf0)) == 0x90) || (((*(event->buffer) & 0xf0)) == 0x80);
}

Note Jack::getNote(const jack_midi_event_t *event) const
{
        Note note = {Whalesynth::NoMIDIKey,
                     Whalesynth::MaxMIDIKeyVelocity,
                     MIDIKeyState::MIDIKeyStateOff};

        if (((*(event->buffer) & 0xf0)) == 0x90) {
                note.midiKeyState    = MIDIKeyState::MIDIKeyStateOn;
                note.midiKeyId       = event->buffer[1];
                note.midiKeyVelocity = event->buffer[2];
        } else if(((*(event->buffer) & 0xf0)) == 0x80) {
                note.midiKeyId       = event->buffer[1];
                note.midiKeyVelocity = event->buffer[2];
        }
        return note;
}

void Jack::processAudio(jack_nframes_t nframes)
{
        float *buffer[2 * Whalesynth::defaultChannelsNumber];
        for (size_t ch = 0; ch < Whalesynth::defaultChannelsNumber; ch++) {
                auto channel = outputChannels[ch];
                buffer[2 * ch] = static_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(channel.first,
                                                                                                nframes));
                memset(buffer[2 * ch], 0, nframes * sizeof(float));
                buffer[2 * ch + 1] = static_cast<jack_default_audio_sample_t*>(jack_port_get_buffer(channel.second,
                                                                                                    nframes));
                memset(buffer[2 * ch + 1], 0, nframes * sizeof(float));
        }

        auto midiBuffer = jack_port_get_buffer(midiInPort, nframes);
	jack_nframes_t eventsCount = jack_midi_get_event_count(midiBuffer);
        jack_nframes_t eventIndex  = 0;
        size_t currentFrame = 0;
        while (eventIndex < eventsCount) {
                jack_midi_event_t event;
                jack_midi_event_get(&event, midiBuffer, eventIndex++);
                auto eventFrame = event.time;
                auto size = eventFrame - currentFrame;
                if (size > 0)
                        whalesynth.process(buffer, size);
                if (isNote(&event))
                        whalesynth.setNote(getNote(&event));
                currentFrame = eventFrame;
        }

        // Process the rest of the buffer after the last event.
        if (currentFrame < nframes)
                whaleSynth.process(buffer, nframes - currentFrame);
}

int Jack::jackProcessCallback(jack_nframes_t nframes, void *arg)
{
        auto obj = static_cast<Jack*>(arg);
        if (obj)
                obj->processAudio(nframes);
        return 0;
}
