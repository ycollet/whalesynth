/**
 * File name: WaveGenerator.h
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

#ifndef GEONSYNTH_WAVE_GENERATOR_H
#define GEONSYNTH_WAVE_GENERATOR_H

#include "WhaleSynth.h"

class WaveGenerator {
 public:
        enum class WaveFunctionType: int {
                WaveFunctionSine     = 0,
                WaveFunctionSquare   = 1,
                WaveFunctionTriangle = 2,
                WaveFunctionSawtooth = 3,
                WaveFunctionNoise    = 4
        };

        WaveGenerator();
        ~WaveGenerator() = default;
        void setWaveFunction(WaveFunctionType type);
        WaveFunctionType waveFunction() const;
        float value(float phase) const;

 protected:
        float sineFunction(float phase) const;
        float squareFunction(float phase) const;
        float triangleFunction(float phase) const;
        float sawtoothFunction(float phase) const;
        float noiseFunction() const;

 private:
        WaveFunctionType generatorWaveFunction;
};

#endif // GEONSYNTH_WAVE_GENERATOR_H
