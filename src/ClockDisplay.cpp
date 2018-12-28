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

#include "ClockDisplay.hpp"
#include "EepromTable.hpp"
#include "application.h"

uint32_t const TIME_FLASH_OFF_MS = 350;
uint32_t const TIME_FLASH_ON_MS = 650;

using namespace nixie_clock;

Timer timerFlashDigits( TIME_FLASH_ON_MS, &ClockDisplay::toggleDigits, ClockDisplay::instance(), true );

//!
//! Public member functions.
//!

ClockDisplay::ClockDisplay(
	bool const ENABLED ) :
		display_( ENABLED )
//		hoursOn_( true ),
//		minutesOn_( true )
{
	std::array< bool, 4 >::iterator i = this->digitFlash_.begin();
	std::array< bool, 4 >::const_iterator const END = this->digitFlash_.end();
	for ( ; i != END; ++i )
	{
		*i = false;
	}

	EEPROM.get( EEPROM_DATE_MODE, this->dateMode_ );
}

ClockDisplay & ClockDisplay::instance()
{
	static ClockDisplay instance;
	return instance;
}

void ClockDisplay::disable()
{
	this->display_.disable();
	this->display_.updateDisplay();
}

void ClockDisplay::disableOptionDisplay()
{
	this->display_.enableDigit( 2 );
}

void ClockDisplay::enable()
{
	this->display_.enable();
	this->display_.updateDisplay();
}

void ClockDisplay::enableOptionDisplay(
	uint8_t const OPTION,
	uint8_t const VALUE )
{
	if ( OPTION <= 9 )
	{
		this->display_.setDigit( 3, OPTION );
		this->display_.disableDigit( 2 );
		ClockDisplay::DigitValues optionDigits = this->decomposeDigits( VALUE );
		this->display_.setDigit( 0, boost::get< 1 >( optionDigits ) );
		this->display_.setDigit( 1, boost::get< 0 >( optionDigits ) );
		this->display_.updateDisplay();
	}
}

void ClockDisplay::flashDisable()
{
	timerFlashDigits.stop();
	this->flashState_ = false;
	this->display_.enableDigit( 0 );
	this->display_.enableDigit( 1 );
	this->display_.enableDigit( 2 );
	this->display_.enableDigit( 3 );
	this->display_.updateDisplay();
}

void ClockDisplay::flashEnable()
{
	timerFlashDigits.start();
	this->flashState_ = true;
}

void ClockDisplay::flashDigit(
	uint8_t const DIGIT,
	bool const ENABLED )
{
	if ( DIGIT < ClockDisplay::NUMBER_DIGITS )
	{
		this->digitFlash_[ DIGIT ] = ENABLED;
	}
}

void ClockDisplay::flashHours(
	bool const ENABLED )
{
	this->digitFlash_[ 2 ] = ENABLED;
	this->digitFlash_[ 3 ] = ENABLED;
}

//! \brief ??
//! \param ENABLED ??
void ClockDisplay::flashMinutes(
	bool const ENABLED )
{
	this->digitFlash_[ 0 ] = ENABLED;
	this->digitFlash_[ 1 ] = ENABLED;
}

bool ClockDisplay::getDateMode()
{
	return this->dateMode_;
}

void ClockDisplay::setDateMode(
	bool const MODE_DAY_FIRST )
{
	this->dateMode_ = MODE_DAY_FIRST;
	EEPROM.put( EEPROM_DATE_MODE, this->dateMode_ );
}

void ClockDisplay::toggleDigits()
{
	if ( this->flashState_ )
	{
		for ( uint8_t i = 0; i < NUMBER_DIGITS; ++i )
		{
			if ( this->digitFlash_[ i ] )
			{
				this->display_.disableDigit( i );
			}
			this->display_.updateDisplay();
		}

		timerFlashDigits.changePeriod( TIME_FLASH_OFF_MS );
		this->flashState_ = false;
	}
	else
	{
		for ( uint8_t i = 0; i < NUMBER_DIGITS; ++i )
		{
			if ( this->digitFlash_[ i ] )
			{
				this->display_.enableDigit( i );
			}
			this->display_.updateDisplay();
		}

		timerFlashDigits.changePeriod( TIME_FLASH_ON_MS );
		this->flashState_ = true;
	}
}

void ClockDisplay::updateDate(
	ClockModel const & CLOCK_MODEL )
{
	if ( this->dateMode_ )
	{
		ClockDisplay::DigitValues monthDigits = this->decomposeDigits( CLOCK_MODEL.getMonth() );
		this->display_.setDigit( 0, boost::get< 1 >( monthDigits ) );
		this->display_.setDigit( 1, boost::get< 0 >( monthDigits ) );

		ClockDisplay::DigitValues dayDigits = this->decomposeDigits( CLOCK_MODEL.getDay() );
		this->display_.setDigit( 2, boost::get< 1 >( dayDigits ) );
		this->display_.setDigit( 3, boost::get< 0 >( dayDigits ) );
	}
	else
	{
		ClockDisplay::DigitValues dayDigits = this->decomposeDigits( CLOCK_MODEL.getDay() );
		this->display_.setDigit( 0, boost::get< 1 >( dayDigits ) );
		this->display_.setDigit( 1, boost::get< 0 >( dayDigits ) );

		ClockDisplay::DigitValues monthDigits = this->decomposeDigits( CLOCK_MODEL.getMonth() );
		this->display_.setDigit( 2, boost::get< 1 >( monthDigits ) );
		this->display_.setDigit( 3, boost::get< 0 >( monthDigits ) );
	}

	this->display_.updateDisplay();
}

void ClockDisplay::updateTime(
	ClockModel const & CLOCK_MODEL )
{
	ClockDisplay::DigitValues minuteDigits = this->decomposeDigits( CLOCK_MODEL.getMinutes() );
	this->display_.setDigit( 0, boost::get< 1 >( minuteDigits ) );
	this->display_.setDigit( 1, boost::get< 0 >( minuteDigits ) );

	ClockDisplay::DigitValues hourDigits = this->decomposeDigits( CLOCK_MODEL.getHours() );
	this->display_.setDigit( 2, boost::get< 1 >( hourDigits ) );
	this->display_.setDigit( 3, boost::get< 0 >( hourDigits ) );

	this->display_.updateDisplay();
}

void ClockDisplay::updateTime(
	TimerModel const & TIMER_MODEL )
{
/*
	ClockDisplay::DigitValues minuteDigits = this->decomposeDigits( TIMER_MODEL.getMinutes() );
	this->display_.setDigit( 0, boost::get< 1 >( minuteDigits ) );
	this->display_.setDigit( 1, boost::get< 0 >( minuteDigits ) );

	ClockDisplay::DigitValues hourDigits = this->decomposeDigits( TIMER_MODEL.getHours() );
	this->display_.setDigit( 2, boost::get< 1 >( hourDigits ) );
	this->display_.setDigit( 3, boost::get< 0 >( hourDigits ) );
*/

	ClockDisplay::DigitValues minuteDigits = this->decomposeDigits( TIMER_MODEL.getSeconds() );
	this->display_.setDigit( 0, boost::get< 1 >( minuteDigits ) );
	this->display_.setDigit( 1, boost::get< 0 >( minuteDigits ) );

	ClockDisplay::DigitValues hourDigits = this->decomposeDigits( TIMER_MODEL.getMinutes() );
	this->display_.setDigit( 2, boost::get< 1 >( hourDigits ) );
	this->display_.setDigit( 3, boost::get< 0 >( hourDigits ) );

	this->display_.updateDisplay();
}

//!
//! Private member functions.
//!

ClockDisplay::DigitValues ClockDisplay::decomposeDigits(
	uint8_t const VALUE )
{
	// See: http://homepage.cs.uiowa.edu/~jones/bcd/divide.html#fixed

	uint8_t q = ( ( VALUE >> 1 ) + VALUE ) >> 1;
	q = ( ( q >> 4 ) + q );
	q >>= 3;

	// Either Q = A/10 or Q+1 = A/10 for all A, so force the remainder into bounds.

	uint8_t r = ( ( q << 2 ) + q ) << 1;
	r = VALUE - r;
	if ( r >= 10 )
	{
		r -= 10;
		q += 1;
	}

	return boost::make_tuple( q, r );
}
