/**
 * File name: geonsynth_lv2_ui.cpp
 * Project: Geonsynth (A software synthesizer)
 *
 * Copyright (C) 2020 Iurie Nistor <http://iuriepage.wordpress.com>
 *
 * This file is part of Geonsynth.
 *
 * Geonsynth is free software; you can redistribute it and/or modify
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

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/ext/atom/atom.h>
#include <lv2/lv2plug.in/ns/ext/atom/util.h>
#include <lv2/lv2plug.in/ns/ext/midi/midi.h>
#include <lv2/lv2plug.in/ns/ext/urid/urid.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#include "MainWindow.h"
#include <RkMain.h>
#include <RkPlatform.h>

#define GEONSYNTH_URI_UI "http://iuriepage.wordpress.com/geonsynth#ui"

static LV2UI_Handle gsynth_instantiate_ui(const LV2UI_Descriptor*   descriptor,
                                         const char*               plugin_uri,
                                         const char*               bundle_path,
                                         LV2UI_Write_Function      write_function,
                                         LV2UI_Controller          controller,
                                         LV2UI_Widget*             widget,
                                         const LV2_Feature* const* features)
{
        if (!features)
                return nullptr;

        void *parent = nullptr;
        LV2UI_Resize *resize = nullptr;
        const LV2_Feature *feature;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_UI__parent))
                        parent = feature->data;

                if (std::string(feature->URI) == std::string(LV2_UI__resize))
                        resize = (LV2UI_Resize*)feature->data;
                features++;
        }

        // Get the info about X Window parent window.
        const uintptr_t parentWinId = (uintptr_t)parent;
        Display* xDisplay = XOpenDisplay(nullptr);
        int screenNumber = DefaultScreen(xDisplay);
        auto info = rk_from_native_x11(xDisplay, screenNumber, parentWinId);

        auto guiApp = new RkMain();
        auto mainWidget = new MainWindow(guiApp, info);
        auto winId = mainWidget->nativeWindowInfo()->window;
        *widget = (LV2UI_Widget)static_cast<uintptr_t>(winId);
        auto size = mainWidget->size();
        resize->ui_resize(resize->handle, size.width(), size.height());
        return static_cast<LV2UI_Handle>(guiApp);
        return nullptr;
}

static void gsynth_cleanup_ui(LV2UI_Handle handle)
{
        if (handle)
                delete static_cast<RkMain*>(handle);
}

static void gsynth_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer )
{
}

static int gsynth_idle(LV2UI_Handle ui)
{
        static_cast<RkMain*>(ui)->exec(false);
        return 0;
}

static const void* gsynth_extension_data(const char* uri)
{
        static const LV2UI_Idle_Interface idleInterface = {gsynth_idle};
        if (std::string(uri) == std::string(LV2_UI__idleInterface))
                return &idleInterface;
    return nullptr;
}

static const LV2UI_Descriptor gsynth_descriptor_ui = {
	GEONSYNTH_URI_UI,
	gsynth_instantiate_ui,
	gsynth_cleanup_ui,
	gsynth_port_event_ui,
	gsynth_extension_data
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:	return &gsynth_descriptor_ui;
	default: return NULL;
        }
}
