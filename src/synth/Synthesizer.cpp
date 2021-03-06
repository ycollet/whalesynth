/**
 * File name: Synthesizer.h
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

#include "Synthesizer.h"
#include "Note.h"

Synthesizer::Synthesizer()
        : channelsNumber{1}
{
        for (size_t i = 0; i < WhaleSynth::MaxMidiKeyId; i++)
                addVoice(std::make_unique<SynthesizerVoice>(static_cast<MIDIKeyId>(i)));
}

void Synthesizer::setNumberOfChannels(size_t n)
{
        channelsNumber = n;
}

size_t Synthesizer::numberOfChannels() const
{
        return channelsNumber;
}

void Synthesizer::setNote(const Note &note)
{
        if (static_cast<size_t>(note.midiKeyId) < synthVoices.size())
                synthVoices[static_cast<size_t>(note.midiKeyId)]->setNote(note);
}

void Synthesizer::process(float** out, size_t size)
{
        for (size_t i = 0; i < synthVoices.size(); i++)
                synthVoices[i]->process(out, size);
}

void Synthesizer::addVoice(std::unique_ptr<SynthesizerVoice> voice)
{
        if (synthVoices.size() < static_cast<decltype(synthVoices.size())>(WhaleSynth::MaxMidiKeyId))
                synthVoices.push_back(std::move(voice));
}

void Synthesizer::setWave(OperatorIndex index, WaveGenerator::WaveFunctionType type)
{
        for (size_t i = 0; i < synthVoices.size(); i++)
                synthVoices[i]->setWave(index, type);
}
