/**
 * File name: SynthesizerVoice.cpp
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

#include "SynthesizerVoice.h"
#include "Note.h"

SynthesizerVoice::SynthesizerVoice(MIDIKeyId keyId)
        : midiKey{keyId}
        , voicePitch{440.0f * pow(2.0f, static_cast<float>(midiKey - 69) / 12.0f)}
{
        size_t n = GeonSynth::NumberOfOperators;
        while (n--) {
                auto op = std::make_unique<Operator>(voicePitch);
                if (n == 1)
                        op->setWave(WaveGenerator::WaveFunctionType::WaveFunctionSawtooth);
                operatorsList.push_back(std::move(op));
        }
}

MIDIKeyId SynthesizerVoice::midiKeyId() const
{
        return midiKey;
}

void SynthesizerVoice::setNote(const Note &note)
{
        GSYNTH_LOG_INFO("state: " << operatorsList.size());
        for (size_t i = 0; i < operatorsList.size(); i++) {
                if (operatorsList[i]->enabled())
                        operatorsList[i]->setOn((note.midiKeyState == MIDIKeyState::MIDIKeyStateOn));
        }
}

void SynthesizerVoice::process(float** out, size_t size)
{
        for (size_t i = 0; i < operatorsList.size(); i++) {
                if (operatorsList[i]->enabled())
                        operatorsList[i]->process(out, size);
        }
}

void SynthesizerVoice::setWave(WaveGenerator::WaveFunctionType type)
{
        for (size_t i = 0; i < operatorsList.size(); i++) {
                if (operatorsList[i]->enabled())
                        operatorsList[i]->setWave(type);
        }
}
