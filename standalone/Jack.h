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

#include "Synthesizer.h"

#include <jack/jack.h>

class Jack {
 public:
        Jack();
        ~Jack();
        bool start();
        void stop();
        bool isActive() const;
        size_t channels() const;
        unsigned int sampleRate() const;

 protected:
        bool createJackClient();
        void createOutputPorts();

 private:
        void updateBufferSize(jack_nframes_t nframes);
        void processAudio(jack_nframes_t nframes);
        static int jackBufferSizeCallback(jack_nframes_t nframes, void *arg);
        static int jackProcessCallback(jack_nframes_t nframes, void *arg);

        jack_client_t *jackClient;
        std::vector<std::pair<jack_port_t*, jack_port_t*>> outputChannels;
        bool jackCreated;
	std::vector<ReductionBuffer<float>> audioBuffers;
};

#endif // JACK_AUDIO_INTERFACE_H
