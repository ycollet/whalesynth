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
        , sineImage(RkImage(175, 38, RK_IMAGE_RC(wave_sine)))
        , squareImage(RkImage(175, 38, RK_IMAGE_RC(wave_square)))
        , triangleImage(RkImage(175, 38, RK_IMAGE_RC(wave_triangle)))
        , sawtoothImage(RkImage(175, 38, RK_IMAGE_RC(wave_sawtooth)))
{
        setSize({214, 71});
        setBackgroundColor({141, 158, 74});
        show();
}

void WaveGeneratorView::setModel(WaveGeneratorModel *model)
{
        waveGeneratorModel = model;
}

WaveGeneratorModel* WaveGeneratorView::model() const
{
        return waveGeneratorModel;
}

void WaveGeneratorView::updateView()
{
}

void WaveGeneratorView::paintEvent(RkPaintEvent *event)
{
        RkPainter painter(this);
        painter.fillRect(rect(), background());
        if (waveGeneratorModel->waveFunction() == WaveFunction::WaveFunctionSine) {
                painter.drawImage(sineImage, (width() - sineImage.width()) / 2,
                                  (height() - sineImage.height()) / 2);
        } else if (waveGeneratorModel->waveFunction() == WaveFunction::WaveFunctionSquare) {
                painter.drawImage(squareImage, (width() - squareImage.width()) / 2,
                                  (height() - squareImage.height()) / 2);
        } else if (waveGeneratorModel->waveFunction() == WaveFunction::WaveFunctionTriangle) {
                painter.drawImage(triangleImage, (width() - triangleImage.width()) / 2,
                                  (height() - triangleImage.height()) / 2);
        } else if (waveGeneratorModel->waveFunction() == WaveFunction::WaveFunctionSawtooth) {
                painter.drawImage(sawtoothImage, (width() - sawtoothImage.width()) / 2,
                                  (height() - sawtoothImage.height()) / 2);
        }
}

void WaveGeneratorView::mouseDoubleClickEvent(RkMouseEvent *event)
{
}

void WaveGeneratorView::bindModel()
{
}

void WaveGeneratorView::unbindModel()
{
}
