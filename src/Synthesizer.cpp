/**
 * File name: Synthesizer.h
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

#include "Synthesizer.h"

Synthesizer::Synthesizer()
        : channelsNumber{1}
        , maxVoiceNumber{128}
{
        for (decltype(GeonSynth::MaxMidiKeyId) i = 0; i < GeonSynth::MaxMidiKeyId; i++)
                addVoice(std::make_unique<SynthesizerVoice(i)>);
}

void Synthesizer::setNumberOfChannels(size_t n)
{
        channelsNumber = n;
}

size_t Synthesizer::numberOfChannels() const
{
        return channelsNumber;
}

void Synthesizer::addNote(Note &note)
{
        auto res = synthVoices.find(note->midiKeyId());
        if (res != synthVoices.end())
                res.second->addNote(note);
}

void Synthesizer::process(float* out, size_t size, ChannelId channel)
{
        GEONSYNTH_UNUSED(out);
        GEONSYNTH_UNUSED(size);
        GEONSYNTH_UNUSED(channel);
}

void Synthesizer::addVoice(std::unique_ptr<SynthesizerVoice> voice)
{
        if (synthVoices.size() < statics_cast<decltype(synthVoices.size())>(GeonSynth::MaxMidiKeyId))
                synthVoices.insert({voice->midKeyId(), std::move(voice)});
}
