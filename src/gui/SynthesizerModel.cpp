/**
 * File name: SynthesizerModel.cpp
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

#include "SynthesizerModel.h"
#include "OperatorModel.h"
#include "SynthesizerProxy.h"

SynthesizerModel::SynthesizerModel(RkObject *parent, SynthesizerProxy *proxy)
        : RkObject(parent)
        , synthProxy{proxy}
{
        for (size_t i = 0; i < WhaleSynth::NumberOfOperators; i++)
                operatorsModels.push_back(new OperatorModel(this, i));
}

SynthesizerModel::~SynthesizerModel()
{
        delete synthProxy;
}

OperatorModel* SynthesizerModel::getOperator(OperatorIndex index) const
{
        if (index < operatorsModels.size())
                return operatorsModels[index];
        return nullptr;
}

const std::vector<OperatorModel*>& SynthesizerModel::operators() const
{
        return operatorsModels;
}

void SynthesizerModel::setOperatorWaveFunction(OperatorIndex index, WaveGenerator::WaveFunctionType waveType)
{
        WHALE_LOG_DEBUG("called");
        synthProxy->setOperatorWaveFunction(index, waveType);
}

WaveGenerator::WaveFunctionType SynthesizerModel::operatorWaveFunction(OperatorIndex index) const
{
        return synthProxy->operatorWaveFunction(index);
}


