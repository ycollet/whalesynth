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

#include "Operator.h"

Operator::Operator(float pitch)
        : operatorPitch{pitch}
        , operatorPhase{0.0f}
        , operatorStarted{false}
        , isEnabled{true}
{
}

void Operator::setPitch(float pitch)
{
        operatorPitch = pitch;
}

float Operator::getPitch() const
{
        return operatorPitch;
}

void Operator::process(float** out, size_t size)
{
        if (operatorStarted) {
                for (size_t i = 0; i < size; i++) {
                        auto val = 0.1f * getValue();
                        out[0][i] += val;
                        out[1][i] += val;
                        incrementPhase();
                }
        }
}

void Operator::setOn(bool b)
{
        GSYNTH_LOG_INFO("operatorStarted : " << b);
        operatorStarted = b;
        if (operatorStarted)
                operatorPhase = 0.0f;
}

float Operator::getValue() const
{
        return waveGenerator.value(operatorPhase);
}

void Operator::incrementPhase()
{
        operatorPhase += (2.0f * M_PI * operatorPitch) / GeonSynth::defaultSampleRate;
        if (operatorPhase > 2.0f * M_PI)
                operatorPhase -= 2.0f * M_PI;
}

void Operator::enable(bool b)
{
        isEnabled = b;
}

bool Operator::enabled() const
{
        return isEnabled;
}

