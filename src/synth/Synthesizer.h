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

#include "GeonSynth.h"
#include "SynthesizerVoice.h"

#ifndef GEONSYNTH_SYNTHESIZER_H
#define GEONSYNTH_SYNTHESIZER_H

class Synthesizer {
 public:
        Synthesizer();
        void setNumberOfChannels(size_t n);
        size_t numberOfChannels() const;
        void setNote(const Note &note);
        void process(float** out, size_t size);

 protected:
        void addVoice(std::unique_ptr<SynthesizerVoice> voice);

 private:
        std::vector<std::unique_ptr<SynthesizerVoice>> synthVoices;
        size_t channelsNumber;
};

#endif // GEONSYNTH_SYNTHESIZER_H
