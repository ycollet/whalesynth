/**
 * File name: main.cpp
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

#include "Jack.h"
#include "Synthesizer.h"
#include "MainWindow.h"

#include <RkMain.h>

int main(int argc, char *argv[])
{
        RkMain mainApp(argc, argv);

        Synthesizer synthesizer;
        Jack jack(synthesizer);

        auto mainWindow = new MainWindow(mainApp);
        jack.start();
        mainWindow->show();
        auto res = mainApp.exec();
        jack.stop();
        return res;
}
