/**
 * File name: UridIdMap.h
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

#ifndef WHALESYNTH_URIDIDMAP_H
#define WHALESYNTH_URIDIDMAP_H

#include "WhaleSynth.h"

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/ext/state/state.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>

constexpr int WHALE_LV2_EVENTS_IN_PORT  = 3;
constexpr int WHALE_LV2_EVENTS_OUT_PORT = 4;
constexpr char WHALESYNTH_URI[] = "http://iuriepage.wordpress.com/whalesynth";
constexpr char WHALESYNTH_URI_STATE[] = "http://iuriepage.wordpress.com/whalesynth#state";
constexpr char WHALESYNTH_URI_STATE_CHANGED[] = "http://lv2plug.in/ns/ext/state#StateChanged";

struct UridIdMap {
        LV2_URID stateId;
        LV2_URID atomChunkId;
        LV2_URID atomSequence;
        LV2_URID atomStateChanged;
        LV2_URID atomObject;
        LV2_URID atomFloat;
        LV2_URID atomInt;
        LV2_URID eventTransfer;
        LV2_URID id;
        LV2_URID operatorId;
        LV2_URID command;
        LV2_URID waveType;
};

inline UridIdMap createUriIdMap(LV2_URID_Map* uridMap)
{
        UridIdMap mapId;
        mapId.stateId          = uridMap->map(uridMap->handle, WHALESYNTH_URI_STATE);
        mapId.atomChunkId      = uridMap->map(uridMap->handle, LV2_ATOM__Chunk);
        mapId.atomSequence     = uridMap->map(uridMap->handle, LV2_ATOM__Sequence);
        mapId.atomStateChanged = uridMap->map(uridMap->handle, WHALESYNTH_URI_STATE_CHANGED);
        mapId.atomObject       = uridMap->map(uridMap->handle, LV2_ATOM__Object);
        mapId.atomFloat        = uridMap->map(uridMap->handle, LV2_ATOM__Float);
        mapId.atomInt          = uridMap->map(uridMap->handle, LV2_ATOM__Int);
        mapId.eventTransfer    = uridMap->map(uridMap->handle, LV2_ATOM__eventTransfer);
        mapId.id               = uridMap->map(uridMap->handle, "WhaleSynth/ID");
        mapId.operatorId       = uridMap->map(uridMap->handle, "WhaleSynth/Operator");
        mapId.command          = uridMap->map(uridMap->handle, "WhaleSynth/Command");
        mapId.waveType         = uridMap->map(uridMap->handle, "WhaleSynth/WaveType");
        return mapId;
};

#endif // WHALESYNTH_URIDIDMAP_H
