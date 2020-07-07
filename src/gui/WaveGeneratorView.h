/**
 * File name: WaveGeneratorView.h
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

#ifndef WAVE_GENERATOR_VIEW_H
#define WAVE_GENERATOR_VIEW_H

#include "WhaleSynth.h"
#include "WhaleWidget.h"

class WaveGeneratorModel;

class WaveGeneratorView: public WhaleWidget {
 public:
        WaveGeneratorView(WhaleWidget* parent, WaveGeneratorModel *model);
        void setModel(WaveGeneratorModel *model);
        WaveGeneratorModel* model() const;
        void updateView();

 protected:
        void bindModel();
        void unbindModel();

 private:
        WaveGeneratorModel *waveGeneratorModel;
};

#endif // WAVE_GENERATOR_VIEW_H
