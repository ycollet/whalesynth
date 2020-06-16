/**
 * File name: Jack.h
 * Project: GeonSynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of GeonSynth.
 *
 * GeonKick is free software; you can redistribute it and/or modify
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

#ifndef GEONSYNTH_JACK_H
#define GEONSYNTH_JACK_H

#include "GeonSynth.h"
#include "Note.h"

#include <jack/jack.h>
#include <jack/midiport.h>

class Synthesizer;

class Jack {
 public:
        Jack(Synthesizer &synthesizer);
        ~Jack();
        bool start();
        void stop();
        bool isActive() const;
        size_t channels() const;
        unsigned int sampleRate() const;

 protected:
        bool createJackClient();
        void createMidiInPot();
        void createOutputPorts();
        bool isNote(const jack_midi_event_t *event) const;
        Note getNote(const jack_midi_event_t *event) const;

 private:
        void processAudio(jack_nframes_t nframes);
        static int jackProcessCallback(jack_nframes_t nframes, void *arg);

        Synthesizer& geonSynth;
        jack_client_t *jackClient;
        jack_port_t* midiInPort;
        bool jackCreated;
        std::vector<std::pair<jack_port_t*, jack_port_t*>> outputChannels;
};

#endif // GEONSYNTH_JACK_H
