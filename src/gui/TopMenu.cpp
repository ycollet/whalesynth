/**
 * File name: TopMenu.cpp
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

#include "TopMenu.h"
#include "SynthesizerModel.h"
#include "OperatorModel.h"

#include <RkContainer.h>

RK_DECLARE_RC(operator_button);
RK_DECLARE_RC(operator_button_hover);
RK_DECLARE_RC(operator_button_on);

TopMenu::TopMenu(WhaleSynthWidget* parent, SynthsizerModel *model)
        : WhaleSynthWidget(parent)
        , synthModel{model}
{
        setFixedSize({parent->width(), 25});
        setBackgroundColor({50, 50, 50});

        auto menuContiner = new RkContainer(this);
        menuContiner->setSize({width(), height() - 3});
        menuContiner->setPosition({0, 3});

        for (const &auto op: synthModel->operators())
                auto button = new RkButton(this);
                button->setSize({24, menuContiner->height()});
                menuContiner->addWidget(button);
                RK_ACT_BIND(button, pressed, RK_ACT_ARGS(), this, showOperator(button, op));
                menuButtons->bush_back(button);
        }

        show();
}

void TopMenu::showOperator(RkButton *button, Operator* op)
{
        for (const auto& btn: menuButtons) {
                if (button != btn)
                        buttons->setPressed(false);
        }
        action operatorSelected(op);
}


