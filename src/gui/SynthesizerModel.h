/**
 * File name: SynthesizerModel.h
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

#ifndef WHALESYNT_SYNTHESIZER_MODEL_H
#define WHALESYNT_SYNTHESIZER_MODEL_H

#include "WhaleSynth.h"
#include "WaveGenerator.h"

#include <RkObject.h>

class SynthesizerProxy;
class OperatorModel;

class SynthesizerModel: public RkObject {
 public:
        SynthesizerModel(RkObject* parent, SynthesizerProxy *proxy);
        ~SynthesizerModel();
        const std::vector<OperatorModel*>& operators() const;
        OperatorModel* getOperator(OperatorIndex index) const;
        void setOperatorWaveFunction(OperatorIndex index, WaveGenerator::WaveFunctionType waveType);
        WaveGenerator::WaveFunctionType operatorWaveFunction(OperatorIndex index) const;

 private:
        SynthesizerProxy *synthProxy;
        std::vector<OperatorModel*> operatorsModels;
};

#endif // WHALESYNT_SYNTHESIZER_MODEL_H
