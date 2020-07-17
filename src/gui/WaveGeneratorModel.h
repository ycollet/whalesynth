/**
 * File name: WaveGeneratorModel.h
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

#ifndef WAVE_GENERATOR_MODEL_H
#define WAVE_GENERATOR_MODEL_H

#include "WhaleSynth.h"

#include <RkObject.h>

class SynthesizerProxy;

class WaveGeneratorModel: public RkObject {
 public:
        WaveGeneratorModel(RkObject* parent, SynthesizerProxy *proxy);
        ~WaveGeneratorModel()= default;

 private:
        SynthesizerProxy *synthProxy;
};

#endif // WAVE_GENERATOR_MODEL_H
