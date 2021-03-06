/**
 * File name: MainWindow.cpp
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

#include "MainWindow.h"
#include "TopMenu.h"
#include "SynthesizerModel.h"
#include "OperatorView.h"

#include <RkContainer.h>

MainWindow::MainWindow(RkMain &app, SynthesizerProxy *synthProxy)
        : WhaleWidget(&app)
        , synthesizerModel{new SynthesizerModel(this, synthProxy)}
        , mainContainer{new RkContainer(this, Rk::Orientation::Vertical)}
{
        setFixedSize(665, 504);
        mainContainer->setSize(size());
        init();
        show();
}

MainWindow::MainWindow(RkMain *app, SynthesizerProxy *synthProxy, const RkNativeWindowInfo& info)
        : WhaleWidget(app, info)
        , synthesizerModel{new SynthesizerModel(this, synthProxy)}
        , mainContainer{new RkContainer(this, Rk::Orientation::Vertical)}
{
        setFixedSize(665, 504);
        mainContainer->setSize(size());
        init();
        show();
}

MainWindow::~MainWindow()
{
}

void MainWindow::init()
{
        auto topMenu = new TopMenu(this, synthesizerModel);
        mainContainer->addWidget(topMenu);

        auto operatorView = new OperatorView(this, synthesizerModel->getOperator(0));
        RK_ACT_BIND(topMenu, operatorSelected, RK_ACT_ARGS(OperatorModel* op), operatorView, setModel(op));
        mainContainer->addWidget(operatorView);
}
