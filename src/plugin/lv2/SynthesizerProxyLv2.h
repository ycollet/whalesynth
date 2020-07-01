/**
 * File name: SynthesizerProxyLv2.h
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

#ifndef SYNTHESIZER_PROXY_LV2_H
#define SYNTHESIZER_PROXY_LV2_H

#include "WhaleSynth.h"
#include "SynthesizerProxy.h"
#include "UridMap.h"

#include <lv2/lv2plug.in/ns/ext/atom/forge.h>
#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

class SynthesizerProxyLv2: public SynthesizerProxy {
 public:
        enum class CommandType: int {
                SetWave = 0
        };

        SynthesizerProxyLv2(LV2UI_Write_Function function,
                            LV2UI_Controller controller,
                            LV2_URID_Map* uridmap);
        ~SynthesizerProxyLv2() = default;
        void setOperatorWaveFunction(const OperatorIndex &index, WaveGenerator::WaveFunctionType type);
        WaveGenerator::WaveFunctionType operatorWaveFunction(const OperatorIndex &index) const;

 protected:
        void writeMessage(LV2_Atom *message);

 private:
        LV2UI_Write_Function writeFunction;
        LV2UI_Controller  uiController;
        LV2_URID_Map* uridMap;
        UridIdMap uridMapId;
        LV2_Atom_Forge atomForge;
        std::vector<uint8_t> stackBuffer;
};

#endif // SYNTHESIZER_PROXY_LV2_H
