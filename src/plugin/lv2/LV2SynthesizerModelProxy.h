/**
 * File name: LV2SynthesizerModelProxy.h
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

#ifndef GEONSYNT_LV2_SYNTHESIZER_MODEL_H
#define GEONSYNT_LV2_SYNTHESIZER_MODEL_H

#include "GeonSynth.h"
#include "SynthesizerModel.h"

#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include <RkObject.h>

class LV2SynthesizerModelProxy : public SynthesizerModel {
 public:
        LV2SynthesizerModelProxy(LV2UI_Write_Function function,
                                 LV2UI_Controller controller,
                                 LV2_URID_Map* uridmap,
                                 RkObject* parent = nullptr);
        ~LV2SynthesizerModelProxy() = default;
        void setWaveFunction(WaveGenerator::WaveFunctionType type);

 protected:
        void sendInt(int value);

 private:
        LV2UI_Write_Function writeFunction;
        LV2UI_Controller  uiController;
        LV2_URID_Map* uridMap;
        LV2_Atom_Forge atomForge;
};

#endif // GEONSYNT_LV2_SYNTHESIZER_MODEL_H