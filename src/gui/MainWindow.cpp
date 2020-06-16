/**
 * File name: MainWindow.cpp
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

#include "MainWindow.h"
#include "OperatorView.h"

MainWindow::MainWindow(RkMain &app)
        : RkWidget(&app)
          synthesizerModel{new SynthesizerModelStandalone(this)}
{
        setFixedSize(800, 500);
        init();
        show();
        GSYNTH_LOG_INFO("called");
}

MainWindow::MainWindow(RkMain *app, SynthesizerModel *model, RkNativeWindowInfo& info)
        : RkWidget(app, info)
        , synthesizerModel{model}
{
        setFixedSize(800, 500);
        synthesizerModel->setEventQueue(eventQueue());
        init();
        show();
        GSYNTH_LOG_INFO("called");
}

MainWindow::~MainWindow()
{
        if (geonkickApi) {
                geonkickApi->registerCallbacks(false);
                geonkickApi->setEventQueue(nullptr);
                if (geonkickApi->isStandalone())
                        delete geonkickApi;
        }
}

void MainWindow::init()
{
        operatorView = new OperatorView(this);
        widget->setPosition(10, 10);
}

MainWindow::~MainWindow()
{
        GSYNTH_LOG_INFO("called");
}
