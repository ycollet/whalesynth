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

#include "Oscillator.h"

Oscillator::Oscillator()
        : oscillatorPitch{440.0f}
        , oscillatorPhase{0.0f}
        , oscillatorStarted{false}
{
}

void Oscillator::setPitch(float pitch)
{
        oscillatorPitch = pitch;
}

float Oscillator::getPitch() const
{
        return oscillatorPitch;
}

void Oscillator::process(float** out, size_t size)
{
        if (oscillatorStarted) {
                for (size_t i = 0; i < size; i++) {
                        auto val = 0.1f * getValue();
                        out[0][i] += val;
                        out[1][i] += val;
                        incrementPhase();
                }
        }
}

void Oscillator::start()
{
        oscillatorStarted = true;
        oscillatorPhase = 0.0f;
}

void Oscillator::stop()
{
        oscillatorStarted = false;
}

float Oscillator::getValue() const
{
        return sin(oscillatorPhase);
}

void Oscillator::incrementPhase()
{
        oscillatorPhase += (2.0f * M_PI * oscillatorPitch) / GeonSynth::defaultSampleRate;
        if (oscillatorPhase > 2.0f * M_PI)
                oscillatorPhase -= 2.0f * M_PI;
}

