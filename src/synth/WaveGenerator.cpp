/**
 * File name: WaveGenerator.cpp
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

#include "WaveGenerator.h"

WaveGenerator::WaveGenerator()
        : generatorWaveFunction{WaveGenerator::WaveFunctionType::WaveFunctionSine}
{
}

void WaveGenerator::setWaveFunction(WaveGenerator::WaveFunctionType type)
{
        generatorWaveFunction = type;
}

WaveGenerator::WaveFunctionType WaveGenerator::waveFunction() const
{
        return generatorWaveFunction;
}

float WaveGenerator::value(float phase) const
{
        switch (generatorWaveFunction) {
        case WaveFunctionType::WaveFunctionSine:
                return sineFunction(phase);
        case WaveFunctionType::WaveFunctionSquare:
                return squareFunction(phase);
        case WaveFunctionType::WaveFunctionTriangle:
                return triangleFunction(phase);
        case WaveFunctionType::WaveFunctionSawtooth:
                 return sawtoothFunction(phase);
        case WaveFunctionType::WaveFunctionNoise:
                return noiseFunction();
        default:
                return sineFunction(phase);
        }
}

float WaveGenerator::sineFunction(float phase) const
{
        return sin(phase);
}

float WaveGenerator::squareFunction(float phase) const
{
        if (phase < M_PI)
                return -1.0f;
        else
                return 1.0f;
}

float WaveGenerator::triangleFunction(float phase) const
{
	if (phase < M_PI)
		return -1.0f + (2.0f / M_PI) * phase;
	else
		return 3.0f - (2.0f / M_PI) * phase;
}

float WaveGenerator::sawtoothFunction(float phase) const
{
        if (phase < M_PI)
                return (1.0f / M_PI) * phase;
        else
                return (1.0f / M_PI) * phase - 2.0f;
}

float WaveGenerator::noiseFunction() const
{
        return 0.0f;
}
