/**
 * File name: LV2SynthesizerModelProxy.cpp
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

#include "GeonSynth.h"
#include "LV2SynthesizerModelProxy.h"

#include <RkObject.h>

LV2SynthesizerModelProxy::LV2SynthesizerModelProxy(LV2UI_Write_Function function,
                                                   LV2UI_Controller controller,
                                                   LV2_URID_Map* uridmap,
                                                   RkObject* parent)
        : SynthesizerModel(parent)
        , writeFunction{function}
        , uiController{controller}
        , uridMap{uridmap}

{
        lv2_atom_forge_init(&atomForge, uridMap);
}

void LV2SynthesizerModelProxy::setWaveFunction(WaveGenerator::WaveFunctionType type)
{
        sendInt(static_cast<int>(type));
}

void LV2SynthesizerModelProxy::sendInt(int value)
{
        LV2_Atom_Forge_Frame forgeFrame;
        lv2_atom_forge_int(&atomForge, value);
        lv2_atom_forge_pop(&atomForge, &forgeFrame);
        //        writeFunction(uiController, 3, sizeof(float), 0, static_cast<const void*>(&value));
}
