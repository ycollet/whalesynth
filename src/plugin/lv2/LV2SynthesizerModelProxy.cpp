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

LV2_URID synthOperatorUrid;
LV2_URID synthID;
LV2_URID synthCommand;
LV2_URID synthSetWave;
LV2_URID synthIntValue;

LV2SynthesizerModelProxy::LV2SynthesizerModelProxy(LV2UI_Write_Function function,
                                                   LV2UI_Controller controller,
                                                   LV2_URID_Map* uridmap,
                                                   RkObject* parent)
        : SynthesizerModel(parent)
        , writeFunction{function}
        , uiController{controller}
        , uridMap{uridmap}
        , uridMapId{getUridMap(uridMap)}
        , stackBuffer(2048, 0)

{
}

void LV2SynthesizerModelProxy::setWaveFunction(WaveGenerator::WaveFunctionType type)
{
        lv2_atom_forge_init(&atomForge, uridMap);
        lv2_atom_forge_set_buffer(&atomForge, stackBuffer.data(), stackBuffer.size());

        LV2_Atom_Forge_Frame frame;

        // Start operator object.
        LV2_Atom *message = (LV2_Atom*)lv2_atom_forge_object(&atomForge, &frame, 0, uridMapId.operator);

        // Add operator ID.
        lv2_atom_forge_key(&atomForge, uridMapId.id);
        lv2_atom_forge_int(&atomForge, 2);

        // Add set wave type command.
        lv2_atom_forge_key(&atomForge, uridMapId.command);
        lv2_atom_forge_int(&atomForge, static_cast<int>(command));

        // Add generator wave type.
        lv2_atom_forge_key(&atomForge, uridMapId.waveType);
        lv2_atom_forge_int(&atomForge, static_cast<int>(type));

        lv2_atom_forge_pop(&atomForge, &frame);

        writeMesasge(message);
}

void LV2SynthesizerModelProxy::writeMessage(LV2_Atom *message)
{
        writeFunction(uiController,
                      GSYNTH_LV2_EVENTS_IN_PORT,
                      lv2_atom_total_size(message),
                      mapId.eventTransfer,
                      msg);
}
