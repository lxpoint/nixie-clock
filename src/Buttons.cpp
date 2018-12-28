/////////////////////////////////////////////////////////////////////////////
//
// Programmable DC load
//
// Copyright (c) 2015 Colin Irwin
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
// For all other enquiries, contact Colin Irwin at lagrangianpoint:
// colin@lagrangianpoint.net.
//
/////////////////////////////////////////////////////////////////////////////

#include "../inc/Buttons.hpp"

namespace nixie_clock
{

char buttonMapping[] = {
    BUTTON_VALUE_0,
    BUTTON_VALUE_1,
    BUTTON_VALUE_2, };
uint8_t buttonPins[] = {
    BUTTON_PIN_0,
    BUTTON_PIN_1,
    BUTTON_PIN_2, };
phi_button_groups buttonGroup( buttonMapping, buttonPins, NUMBER_BUTTONS );

void initialiseButtons()
{
	pinMode( A2, INPUT );
	pinMode( A3, INPUT );
	pinMode( A4, INPUT );
}

}
