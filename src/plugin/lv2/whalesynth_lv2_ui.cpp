/**
 * File name: whalesynth_lv2_ui.cpp
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

#include "MainWindow.h"
#include "LV2SynthesizerModelProxy.h"

#include <RkMain.h>
#include <RkPlatform.h>

#define WHALESYNTH_URI_UI "http://iuriepage.wordpress.com/whalesynth#ui"

class Lv2UiPlugin {
public:
        Lv2UiPlugin(const LV2_Feature* const* features,
                    LV2UI_Write_Function function,
                    LV2UI_Controller controller,
                    LV2UI_Widget *widget,
                    LV2_URID_Map *map)
                : guiApp{nullptr}
                , mainWidget{nullptr}
        {
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

                // Get paret window info.
                const uintptr_t parentWinId = (uintptr_t)parent;
                Display* xDisplay = XOpenDisplay(nullptr);
                int screenNumber = DefaultScreen(xDisplay);
                auto info = rk_from_native_x11(xDisplay, screenNumber, parentWinId);

                // Create GUI
                guiApp = new RkMain();
                auto synthesizerModel = new LV2SynthesizerModelProxy(function, controller, map);
                mainWidget = new MainWindow(guiApp, synthesizerModel, info);

                auto winId = mainWidget->nativeWindowInfo()->window;
                *widget = (LV2UI_Widget)static_cast<uintptr_t>(winId);
                auto size = mainWidget->size();
                resize->ui_resize(resize->handle, size.width(), size.height());
        }

        ~Lv2UiPlugin()
        {
                closeGui();
        }

        RkMain *getGuiApp()
        {
                return guiApp;
        }

        void closeGui()
        {
                WHALE_LOG_INFO("called");
                if (guiApp)
                        delete guiApp;
        }

        void exect()
        {
                if (guiApp)
                        guiApp->exec(false);
        }

private:
        RkMain *guiApp;
        MainWindow *mainWidget;
};

static LV2UI_Handle whale_instantiate_ui(const LV2UI_Descriptor*   descriptor,
                                         const char*               plugin_uri,
                                         const char*               bundle_path,
                                         LV2UI_Write_Function      write_function,
                                         LV2UI_Controller          controller,
                                         LV2UI_Widget*             widget,
                                         const LV2_Feature* const* features)
{
        WHALE_UNUSED(descriptor);
        WHALE_UNUSED(plugin_uri);
        WHALE_UNUSED(bundle_path);
        WHALE_LOG_DEBUG("THREADID:" << std::this_thread::get_id());
        const LV2_Feature *feature;
        LV2_URID_Map *uridMap = nullptr;
        while ((feature = *features)) {
                if (std::string(feature->URI) == std::string(LV2_URID__map)) {
                        uridMap = static_cast<LV2_URID_Map*>(feature->data);
                        break;
                }
                features++;
        }

        if (!uridMap) {
                WHALE_LOG_ERROR("can't find urid map");
                return nullptr;
        }

        Lv2UiPlugin *uiPlugin = nullptr;
        if (features) {
                uiPlugin = new Lv2UiPlugin(features,
                                           write_function,
                                           controller,
                                           widget,
                                           uridMap);
                return static_cast<LV2UI_Handle>(uiPlugin);
        }
        return nullptr;
}

static void whale_cleanup_ui(LV2UI_Handle handle)
{
        WHALE_LOG_INFO("called");
        if (handle)
                delete static_cast<Lv2UiPlugin*>(handle);
}

static void whale_port_event_ui(LV2UI_Handle ui,
                                uint32_t port_index,
                                uint32_t buffer_size,
                                uint32_t format,
                                const void *buffer)
{
        WHALE_LOG_INFO("called, port_index: " << port_index);
}

static int whale_idle(LV2UI_Handle ui)
{
        auto plugin = static_cast<Lv2UiPlugin*>(ui);
        if (plugin)
                plugin->exect();
        return 0;
}

static const void* whale_extension_data(const char* uri)
{
        static const LV2UI_Idle_Interface idleInterface = {whale_idle};
        if (std::string(uri) == std::string(LV2_UI__idleInterface))
                return &idleInterface;
        return nullptr;
}

static const LV2UI_Descriptor whale_descriptor_ui = {
	WHALESYNTH_URI_UI,
	whale_instantiate_ui,
	whale_cleanup_ui,
	whale_port_event_ui,
	whale_extension_data
};

const LV2UI_Descriptor* lv2ui_descriptor(uint32_t index)
{
	switch (index)
        {
	case 0:	return &whale_descriptor_ui;
	default: return NULL;
        }
}
