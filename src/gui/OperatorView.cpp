/**
 * File name: OperatorView.cpp
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

#include "OperatorView.h"
#include "WaveGenerator.h"

#include "RkButton.h"

OperatorView::OperatorView(RkWidget* parent, SynthesizerModel *synth)
        : RkWidget(parent)
        , synthModel{synth}
{
        setBackgroundColor({123, 56, 100});
        setFixedSize(200, 150);
        auto button = new RkButton(this);
        button->setSize(24, 24);
        button->setBackgroundColor({0, 100, 0});
        button->setPosition(10, 10);
        RK_ACT_BIND(button, pressed, RK_ACT_ARGS(), this, setWaveFunction());
        button->show();
        show();
}

void OperatorView::setWaveFunction()
{
        static int wave = 0;
        if (wave > 4)
                wave = 0;
        WHALE_LOG_INFO("called");
        synthModel->setWaveFunction(static_cast<WaveGenerator::WaveFunctionType>(wave));
        wave++;
}
