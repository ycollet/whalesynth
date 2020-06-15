/**
 * File name: Operator.h
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

#ifndef GEONSYNTH_OPERATOR_H
#define GEONSYNTH_OPERATOR_H

#include "GeonSynth.h"
#include "WaveGenerator.h"

class Operator {
 public:
        Operator(float pitch = GeonSynth::defaultPitch);
        ~Operator() = default;
        void setPitch(float pitch);
        float getPitch() const;
        void process(float** out, size_t size);
        void setOn(bool b = true);
        void enable(bool b = true);
        bool enabled() const;
        void setWave(WaveGenerator::WaveFunctionType type) { waveGenerator.setWaveFunction(type); }

 protected:
        float getValue() const;
        void incrementPhase();

 private:
        float operatorPitch;
        float operatorPhase;
        bool operatorStarted;
        bool isEnabled;
        WaveGenerator waveGenerator;
};

#endif // GEONSYNTH_OPERATOR_H
