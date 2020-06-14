/**
 * File name: Oscillator.h
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

#ifndef GEONSYNTH_OSCILLATOR_H
#define GEONSYNTH_OSCILLATOR_H

#include "GeonSynth.h"

class Oscillator {
 public:
        Oscillator();
        ~Oscillator() = default;
        void setPitch(float pitch);
        float getPitch() const;
        void process(float** out, size_t size);
        void start();
        void stop();

 protected:
        float getValue() const;
        void incrementPhase();

 private:
        float oscillatorPitch;
        float oscillatorPhase;
        bool oscillatorStarted;
};

#endif // GEONSYNTH_OSCILLATOR_H
