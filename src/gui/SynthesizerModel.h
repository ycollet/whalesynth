/**
 * File name: SynthesizerModel.h
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

#ifndef GEONSYNT_SYNTHESIZER_MODEL_H
#define GEONSYNT_SYNTHESIZER_MODEL_H

#include "GeonSynth.h"
#include "WaveGenerator.h"

#include <RkObject.h>

class SynthesizerModel: public RkObject {
 public:
        SynthesizerModel(RkObject* parent);
        virtual ~SynthesizerModel() = default;
        virtual void setWaveFunction(WaveGenerator::WaveFunctionType type) = 0;
};

#endif // GEONSYNT_SYNTHESIZER_MODEL_H