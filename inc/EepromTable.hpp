/////////////////////////////////////////////////////////////////////////////
//
// Nixie Clock
//
// Copyright (c) 2017 Colin Irwin
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

#if !defined( NIXIE_CLOCK_EEPROM_TABLE_HPP )
#define NIXIE_CLOCK_EEPROM_TABLE_HPP

namespace nixie_clock
{

//! \brief ??
static int const EEPROM_BACKLIGHT_ENABLE		= 0;

//! \brief ??
static int const EEPROM_BACKLIGHT_BRIGHTNESS	= 1;

//! \brief ??
static int const EEPROM_BACKLIGHT_BLUE			= 2;

//! \brief ??
static int const EEPROM_BACKLIGHT_GREEN			= 3;

//! \brief ??
static int const EEPROM_BACKLIGHT_RED 			= 4;

//! \brief ??
static int const EEPROM_TIME_MODE				= 5;

//! \brief ??
static int const EEPROM_DATE_MODE				= 6;

}

#endif
