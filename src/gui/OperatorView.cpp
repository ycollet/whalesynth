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
#include "OperatorModel.h"
#include "WaveGeneratorView.h"

OperatorView::OperatorView(WhaleWidget* parent, OperatorModel *model)
        : WhaleWidget(parent)
        , operatorModel{model}
        , waveGeneratorView{new WaveGeneratorView(this, (model ? model->waveGeneratorModel() : nullptr))}
{
        setBackgroundColor({82, 82, 82});
        setFixedSize(parent->width(), 168);

        auto mainContiner = new RkContiner(this);
        mainContiner->setSize(size());
        auto leftContiner = new RkContiner(this, Rk::Orientation::Vertical)
        leftContiner->setSize({waveGeneratorView->width(), mainContiner->height()});
        mainContiner->addContainer(leftContiner);
        leftContiner->addWidget(waveGeneratorView);

        bindModel();
        updateView();
        show();
}

void OperatorView::updateView()
{
}

void OperatorView::setModel(OperatorModel *model)
{
        unbindModel();
        operatorModel = model;
        bindModel();
        updateView();
}

void OperatorView::bindModel()
{
}

void OperatorView::unbindModel()
{
}
