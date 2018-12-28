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

#include "ClockModel.hpp"
#include "EepromTable.hpp"
#include "application.h"

using namespace nixie_clock;

ClockModel::ClockModel(
	bool const MODE_24_HOUR ) :
		mode24Hour_( MODE_24_HOUR )
{
	Time.zone( 1 );
	EEPROM.get( EEPROM_TIME_MODE, this->mode24Hour_ );
	this->update();
}

ClockModel & ClockModel::instance()
{
	static ClockModel instance;
	return instance;
}

uint8_t ClockModel::getDay() const
{
	return Time.day();
}

uint8_t ClockModel::getHours() const
{
	return this->mode24Hour_ ? Time.hour() : Time.hourFormat12();
}

uint8_t ClockModel::getMinutes() const
{
	return Time.minute();
}

bool ClockModel::getMode() const
{
	return this->mode24Hour_;
}

uint8_t ClockModel::getMonth() const
{
	return Time.month();
}

uint8_t ClockModel::getSeconds() const
{
	return Time.second();
}

uint16_t ClockModel::getYear() const
{
	return Time.year();
}

void ClockModel::setMode(
	bool const MODE_24_HOUR )
{
	this->mode24Hour_ = MODE_24_HOUR;
	EEPROM.put( EEPROM_TIME_MODE, this->mode24Hour_ );
}

void ClockModel::update() const
{
	this->isDST() ? Time.beginDST() : Time.endDST();
}

bool ClockModel::isDST() const
{
	// (Central) European Summer Timer calculation (last Sunday in March/October)
	int dayOfMonth = Time.day();
	int month = Time.month();
	int dayOfWeek = Time.weekday() - 1; // make Sunday 0 .. Saturday 6

	if ( ( month >= 4 ) && ( month <= 9 ) )
	{
		// April to September definetly DST
		return true;
	}
	else if ( ( month < 3 ) || ( month > 10 ) )
	{
		// before March or after October is definetly standard time
		return false;
	}

	// March and October need deeper examination
	bool lastSundayOrAfter = ( ( dayOfMonth - dayOfWeek ) > 24 );
	if ( !lastSundayOrAfter )
	{
		// Before switching Sunday, October DST will be true, March not.
		return ( month == 10 );
	}

	if (dayOfWeek)
	{
		// After switching Sunday, March DST is true, for October not.
		return ( month == 3 );
	}

	int secSinceMidnightUTC = Time.now() % 86400;
	boolean dayStartedAs = ( month == 10 ); // DST in October, in March not
	// on switching Sunday we need to consider the time
	if ( secSinceMidnightUTC >= 3600 )
	{
		// 1:00 UTC (=1:00 GMT/2:00 BST or 2:00 CET/3:00 CEST)
		return !dayStartedAs;
	}

	return dayStartedAs;
}
