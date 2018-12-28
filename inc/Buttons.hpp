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

#if !defined( NIXIE_CLOCK_BUTTONS_HPP )
#define NIXIE_CLOCK_BUTTONS_HPP

#include "Arduino.h"
#include "../inc/phi_interfaces.h"

namespace nixie_clock
{

uint8_t const NUMBER_BUTTONS		= 3;

char const BUTTON_VALUE_0           = 0x01;
char const BUTTON_VALUE_1           = 0x02;
char const BUTTON_VALUE_2           = 0x03;
uint8_t const BUTTON_PIN_0          = A4;
uint8_t const BUTTON_PIN_1          = A3;
uint8_t const BUTTON_PIN_2          = A2;

extern char buttonMapping[];
extern uint8_t buttonPins[];
extern phi_button_groups buttonGroup;

void initialiseButtons();

}

#endif
