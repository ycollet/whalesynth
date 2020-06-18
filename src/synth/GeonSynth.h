/**
 * File name: GeonSynth.h
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

#ifndef GEONSYNTH_H
#define GEONSYNTH_H

#include <string>
#include <memory>
#include <vector>
#include <iostream>
#include <math.h>

#define GSYNTH_UNUSED(expr) (void)expr

constexpr unsigned int GEONSYNTH_VERSION = 0x010000;
constexpr char GEONSYNTH_NAME[] = "GeonSynth";
constexpr char GEONSYNTH_APP_NAME[] = "geonsynth";
constexpr char GEONSYNTH_VERSION_STRING[] = "1.0.0";

using AudioFrame = int;
using MIDIKeyId = char;
using MIDIKeyVelocity = char;
enum class MIDIKeyState : int {
        MIDIKeyStateOff = 0,
        MIDIKeyStateOn  = 1
};

using ChannelId = int;

namespace GeonSynth {
        constexpr AudioFrame NoAudioFrame = -1;
        constexpr ChannelId AllChannels = -1;
        constexpr MIDIKeyId MaxMidiKeyId = 127;
        constexpr MIDIKeyId NoMIDIKey = -1;
        constexpr MIDIKeyVelocity MaxMIDIKeyVelocity = 127;

        // Default stereo channels number.
        constexpr size_t defaultChannelsNumber = 1;

        constexpr unsigned int defaultSampleRate = 48000;

        constexpr float defaultPitch = 440.0f;

        constexpr MIDIKeyVelocity NumberOfOperators = 8;
};

#ifdef GEONSYNTH_DEBUG
#define GSYNTH_LOG_INFO(msg) std::cout << __PRETTY_FUNCTION__ << "[INFO] " << msg << std::endl;
#define GSYNTH_LOG_ERROR(msg) std::cout << __PRETTY_FUNCTION__ << "[ERROR] " << msg << std::endl;
#define GSYNTH_LOG_DEBUG(msg) std::cout << __PRETTY_FUNCTION__ << "[DEBUG] " << msg << std::endl;
#else
#define GSYNTH_LOG_INFO(msg) std::cout << "[INFO] " << msg << std::endl;
#define GSYNTH_LOG_ERROR(msg) std::cout << "[ERROR] " << msg << std::endl;
#define GSYNTH_LOG_DEBUG(msg)
#endif // GEONSYNTH_DEBUG

#endif // GEONSYNTH_H
