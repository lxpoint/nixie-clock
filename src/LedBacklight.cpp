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

#include "LedBacklight.hpp"
#include "EepromTable.hpp"
#include "Arduino.h"

// Output lines for the register ICs.
static const int LED_B	= D2;
static const int LED_G	= D1;
static const int LED_R	= D0;

using namespace nixie_clock;

LedBacklight::LedBacklight(
	bool const ENABLED ) :
		blue_( 0 ),
		brightness_( 0 ),
		enabled_( ENABLED ),
		green_( 0 ),
		red_( 0 )
{
	pinMode( LED_B,	OUTPUT );
	pinMode( LED_G,	OUTPUT );
	pinMode( LED_R,	OUTPUT );
}

LedBacklight & LedBacklight::instance()
{
	static LedBacklight instance;
	return instance;
}

void LedBacklight::decrementBlue()
{
	if ( this->blue_ > 0 )
	{
		this->blue_ -= 1;
		EEPROM.put( EEPROM_BACKLIGHT_BLUE, this->blue_ );
		this->updateBlue();
	}
}

void LedBacklight::decrementBrightness()
{
	if ( this->brightness_ > 0 )
	{
		this->brightness_ -= 1;
		EEPROM.put( EEPROM_BACKLIGHT_BRIGHTNESS, this->brightness_ );
		this->updateBlue();
		this->updateGreen();
		this->updateRed();
	}
}

void LedBacklight::decrementGreen()
{
	if ( this->green_ > 0 )
	{
		this->green_ -= 1;
		EEPROM.put( EEPROM_BACKLIGHT_GREEN, this->green_ );
		this->updateGreen();
	}
}

void LedBacklight::decrementRed()
{
	if ( this->red_ > 0 )
	{
		this->red_ -= 1;
		EEPROM.put( EEPROM_BACKLIGHT_RED, this->red_ );
		this->updateRed();
	}
}

void LedBacklight::disable()
{
	this->enabled_ = false;
	EEPROM.put( EEPROM_BACKLIGHT_ENABLE, this->enabled_ );
	analogWrite( LED_B, 0, FREQUENCY_LED_PWM );
	analogWrite( LED_G, 0, FREQUENCY_LED_PWM );
	analogWrite( LED_R, 0, FREQUENCY_LED_PWM );
}

void LedBacklight::enable()
{
	this->enabled_ = true;
	EEPROM.put( EEPROM_BACKLIGHT_ENABLE, this->enabled_ );
	this->updateBlue();
	this->updateGreen();
	this->updateRed();
}

bool LedBacklight::enabled()
{
	return this->enabled_;
}

uint8_t LedBacklight::getBrightness()
{
	return this->brightness_;
}

uint8_t LedBacklight::getBlue()
{
	return this->blue_;
}

uint8_t LedBacklight::getGreen()
{
	return this->green_;
}

uint8_t LedBacklight::getRed()
{
	return this->red_;
}

void LedBacklight::incrementBlue()
{
	if ( this->blue_ < LedBacklight::INTENSITY_MAX )
	{
		this->blue_ += 1;
		EEPROM.put( EEPROM_BACKLIGHT_BLUE, this->blue_ );
		this->updateBlue();
	}
}

void LedBacklight::incrementBrightness()
{
	if ( this->brightness_ < LedBacklight::INTENSITY_MAX )
	{
		this->brightness_ += 1;
		EEPROM.put( EEPROM_BACKLIGHT_BRIGHTNESS, this->brightness_ );
		this->updateBlue();
		this->updateGreen();
		this->updateRed();
	}
}

void LedBacklight::incrementGreen()
{
	if ( this->green_ < LedBacklight::INTENSITY_MAX )
	{
		this->green_ += 1;
		EEPROM.put( EEPROM_BACKLIGHT_GREEN, this->green_ );
		this->updateGreen();
	}
}

void LedBacklight::incrementRed()
{
	if ( this->red_ < LedBacklight::INTENSITY_MAX )
	{
		this->red_ += 1;
		EEPROM.put( EEPROM_BACKLIGHT_RED, this->red_ );
		this->updateRed();
	}
}

void LedBacklight::loadState()
{
	EEPROM.get( EEPROM_BACKLIGHT_ENABLE, this->enabled_ );
	EEPROM.get( EEPROM_BACKLIGHT_BRIGHTNESS, this->brightness_ );
	EEPROM.get( EEPROM_BACKLIGHT_BLUE, this->blue_ );
	EEPROM.get( EEPROM_BACKLIGHT_GREEN, this->green_ );
	EEPROM.get( EEPROM_BACKLIGHT_RED, this->red_ );
	this->updateBlue();
	this->updateGreen();
	this->updateRed();
}

void LedBacklight::saveState()
{
	EEPROM.put( EEPROM_BACKLIGHT_ENABLE, this->enabled_ );
	EEPROM.put( EEPROM_BACKLIGHT_BRIGHTNESS, this->brightness_ );
	EEPROM.put( EEPROM_BACKLIGHT_BLUE, this->blue_ );
	EEPROM.put( EEPROM_BACKLIGHT_GREEN, this->green_ );
	EEPROM.put( EEPROM_BACKLIGHT_RED, this->red_ );
}

void LedBacklight::setBrightness(
	uint8_t const BRIGHTNESS )
{
	if ( this->brightness_ <= LedBacklight::INTENSITY_MAX )
	{
		this->brightness_ += BRIGHTNESS;
	}
}

void LedBacklight::updateBlue()
{
	if ( this->enabled_ )
	{
		uint16_t blueLevel = ( this->blue_ * this->brightness_ * LedBacklight::INTENSITY_STEP ) / INTENSITY_MAX;
		if ( blueLevel == LED_VALUE_MAX )
//		if ( this->brightness_ == LedBacklight::INTENSITY_MAX )
		{
			analogWrite( LED_B, 0xff, FREQUENCY_LED_PWM );
		}
		else
		{
			analogWrite( LED_B, blueLevel, FREQUENCY_LED_PWM );
		}
	}
}

void LedBacklight::updateGreen()
{
	if ( this->enabled_ )
	{
		uint16_t greenLevel = ( this->green_ * this->brightness_ * LedBacklight::INTENSITY_STEP ) / INTENSITY_MAX;
		if ( greenLevel == LED_VALUE_MAX )
//		if ( this->brightness_ == LedBacklight::INTENSITY_MAX )
		{
			analogWrite( LED_G, 0xff, FREQUENCY_LED_PWM );
		}
		else
		{
			analogWrite( LED_G, greenLevel, FREQUENCY_LED_PWM );
		}
	}
}

void LedBacklight::updateRed()
{
	if ( this->enabled_ )
	{
		uint16_t redLevel = ( this->red_ * this->brightness_ * LedBacklight::INTENSITY_STEP ) / INTENSITY_MAX;
		if ( redLevel == LED_VALUE_MAX )
//		if ( this->brightness_ == LedBacklight::INTENSITY_MAX )
		{
			analogWrite( LED_R, 0xff, FREQUENCY_LED_PWM );
		}
		else
		{
			analogWrite( LED_R, redLevel, FREQUENCY_LED_PWM );
		}
	}
}
