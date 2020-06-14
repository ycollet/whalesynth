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
//#include "Oscillator.h"
#include "Note.h"

SynthesizerVoice::SynthesizerVoice(MIDIKeyId keyId)
        : midiKey{keyId}
          //        , voiceOscillator{std::make_unique<Oscillator>()}
{
        voiceOscillator.setPitch(440.0f * pow(2.0f, static_cast<float>(midiKey - 69) / 12.0f));
}

MIDIKeyId SynthesizerVoice::midiKeyId() const
{
        return midiKey;
}

void SynthesizerVoice::setNote(const Note &note)
{
        GSYNTH_LOG_INFO("note[" << static_cast<int>(note.midiKeyId) << "]: " << static_cast<int>(note.midiKeyState));
        if (note.midiKeyState == MIDIKeyState::MIDIKeyStateOn)
                voiceOscillator.start();
        else
                voiceOscillator.stop();
}

void SynthesizerVoice::process(float** out, size_t size)
{
        voiceOscillator.process(out, size);
}
