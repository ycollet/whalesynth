/**
 * File name: WaveGeneratorView.cpp
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

#include "WaveGeneratorView.h"
#include "WaveGeneratorModel.h"

RK_DECLARE_IMAGE_RC(wave_sine);
RK_DECLARE_IMAGE_RC(wave_square);
RK_DECLARE_IMAGE_RC(wave_triangle);
RK_DECLARE_IMAGE_RC(wave_sawtooth);

WaveGeneratorView::WaveGeneratorView(WhaleWidget* parent,
                                     WaveGeneratorModel *model)
        : WhaleWidget(parent)
        , waveGeneratorModel{model}
        , waveFunctionImages {
                {static_cast<int>(WaveFunction::WaveFunctionSine), RkImage(175, 38, RK_IMAGE_RC(wave_sine))},
                {static_cast<int>(WaveFunction::WaveFunctionSquare), RkImage(175, 38, RK_IMAGE_RC(wave_square))},
                {static_cast<int>(WaveFunction::WaveFunctionTriangle), RkImage(175, 38, RK_IMAGE_RC(wave_triangle))},
                {static_cast<int>(WaveFunction::WaveFunctionSawtooth), RkImage(175, 38, RK_IMAGE_RC(wave_sawtooth))}}
        , currentFunctionIndex{static_cast<int>(waveGeneratorModel->waveFunction())}
{
        setSize({214, 71});
        setBackgroundColor({141, 158, 74});
        show();
}

void WaveGeneratorView::setModel(WaveGeneratorModel *model)
{
        waveGeneratorModel = model;
        currentFunctionIndex = static_cast<int>(waveGeneratorModel->waveFunction());
}

WaveGeneratorModel* WaveGeneratorView::model() const
{
        return waveGeneratorModel;
}

void WaveGeneratorView::updateView()
{
        update();
}

void WaveGeneratorView::paintEvent(RkPaintEvent *event)
{
        RkPainter painter(this);
        painter.fillRect(rect(), background());
        auto res = waveFunctionImages.find(static_cast<int>(waveGeneratorModel->waveFunction()));
        if (res != waveFunctionImages.end()) {
                painter.drawImage(res->second, (width() - res->second.width()) / 2,
                                  (height() - res->second.height()) / 2);
        }
}

void WaveGeneratorView::mouseDoubleClickEvent(RkMouseEvent *event)
{
        currentFunctionIndex++;
        if (currentFunctionIndex > static_cast<int>(WaveFunction::WaveFunctionSawtooth))
                currentFunctionIndex = static_cast<int>(WaveFunction::WaveFunctionSine);
        WHALE_LOG_DEBUG("called");
        waveGeneratorModel->setWaveFunction(static_cast<WaveFunction>(currentFunctionIndex));
}

void WaveGeneratorView::bindModel()
{
}

void WaveGeneratorView::unbindModel()
{
}
