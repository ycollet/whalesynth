/**
 * File name: UridMap.h
 * Project: Whalesynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Whalesynth.
 *
 * Whalesynth is free software; you can redistribute it and/or modify
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

#ifndef GEONSYNTH_URIDMAP_H
#define GEONSYNTH_URIDMAP_H

constexpr int WHALE_LV2_EVENTS_IN_PORT  = 3;
constexpr int WHALE_LV2_EVENTS_OUT_PORT = 4;

struct UridMapId {
        stateId;
        atomChunkId;
        atomSequence;
        atomStateChanged;
        atomObject;
        atomFloat;
        atomInt;

        mapId.id;
        mapId.operator;
        mapId.command;
        mapId.waveType;
};

UridMapId getUridMap(LV2_URIDMap* map)
{
        UridMapId mapId;
        mapId.stateId          = uridMap->map(uridMap->handle, GEONSYNTH_URI_STATE);
        mapId.atomChunkId      = uridMap->map(uridMap->handle, LV2_ATOM__Chunk);
        mapId.atomSequence     = uridMap->map(uridMap->handle, LV2_ATOM__Sequence);
        mapId.atomStateChanged = uridMap->map(uridMap->handle, GEONSYNTH_URI_STATE_CHANGED);
        mapId.atomObject       = uridMap->map(uridMap->handle, LV2_ATOM__Object);
        mapId.atomFloat        = uridMap->map(uridMap->handle, LV2_ATOM__Float);
        mapId.atomInt          = uridMap->map(uridMap->handle, LV2_ATOM__Int);
        mapId.eventTransfer    = uridMap->map(uridMap->handle, LV2_ATOM__eventTransfer);

        mapId.id       = uridMap->map(uridMap->handle, "WhaleSynth/ID");
        mapId.operator = uridMap->map(uridMap->handle, "WhaleSynth/Operator");
        mapId.command  = uridMap->map(uridMap->handle, "WhaleSynth/Command");
        mapId.waveType = uridMap->map(uridMap->handle, "WhaleSynth/WaveType");

        return getUridMap;
};

#define // GEONSYNTH_URIDMAP_H
