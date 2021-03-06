/**
 * File name: SynthesizerVoice.h
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

#ifndef SYNTHESIZER_VOICE_H
#define SYNTHESIZER_VOICE_H

#include "WhaleSynth.h"
#include "Operator.h"

//class Operator;
class Note;

class SynthesizerVoice {
 public:
        SynthesizerVoice(MIDIKeyId keyId = WhaleSynth::NoMIDIKey);
        ~SynthesizerVoice() = default;
        MIDIKeyId midiKeyId() const;
        void setNote(const Note &note);
        void process(float** out, size_t size);
        void setWave(OperatorIndex index, WaveGenerator::WaveFunctionType type);
        bool isActive() const;
        void setActive(bool b);

 private:
        MIDIKeyId midiKey;
        float voicePitch;
        std::vector<std::unique_ptr<Operator>> operatorsList;
        bool voiceIsActive;
};

#endif // SYNTHESIZER_VOICE_H
