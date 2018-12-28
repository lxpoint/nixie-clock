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

#include "DisplayDecoder.hpp"
#include "Arduino.h"

// Useful constants given the control lines run through an inverter.
static int const AL_HIGH		= LOW;
static int const AL_LOW			= HIGH;

// Output lines for the register chips.
static const int RCK 			= D5;
static const int SER_IN			= D3;
static const int SRCK 			= D4;

// Digit coding constants.
static uint8_t const D_0		= 0x00;
static uint8_t const D_1		= 0x01;
static uint8_t const D_2		= 0x02;
static uint8_t const D_3		= 0x03;
static uint8_t const D_4		= 0x04;
static uint8_t const D_5		= 0x05;
static uint8_t const D_6		= 0x06;
static uint8_t const D_7		= 0x07;
static uint8_t const D_8		= 0x08;
static uint8_t const D_9		= 0x09;
static uint8_t const D_LDP		= 0x0a;
static uint8_t const D_RDP		= 0x0b;

using namespace nixie_clock;

DisplayDecoder::DisplayDecoder(
	bool const ENABLE_STATE ) :
		enabled_( ENABLE_STATE )
{
	pinMode( RCK, 		OUTPUT );
	pinMode( SER_IN,	OUTPUT );
	pinMode( SRCK, 		OUTPUT );

	digitalWrite( RCK, 		AL_LOW );
	digitalWrite( SER_IN, 	AL_LOW );
	digitalWrite( SRCK, 	AL_LOW );

	this->digitStates_[ 0 ] = ENABLE_STATE;
	this->digitStates_[ 1 ] = ENABLE_STATE;
	this->digitStates_[ 2 ] = ENABLE_STATE;
	this->digitStates_[ 3 ] = ENABLE_STATE;

	this->digitValues_[ 0 ] = 0;
	this->digitValues_[ 1 ] = 0;
	this->digitValues_[ 2 ] = 0;
	this->digitValues_[ 3 ] = 0;

//	0x00000000f0ff
	this->display_[ 0 ][   D_0 ] = DisplayBits(	0x000000000020 );
	this->display_[ 0 ][   D_1 ] = DisplayBits(	0x000000004000 );
	this->display_[ 0 ][   D_2 ] = DisplayBits(	0x000000008000 );
	this->display_[ 0 ][   D_3 ] = DisplayBits(	0x000000002000 );
	this->display_[ 0 ][   D_4 ] = DisplayBits(	0x000000001000 );
	this->display_[ 0 ][   D_5 ] = DisplayBits(	0x000000000001 );
	this->display_[ 0 ][   D_6 ] = DisplayBits(	0x000000000002 );
	this->display_[ 0 ][   D_7 ] = DisplayBits(	0x000000000004 );
	this->display_[ 0 ][   D_8 ] = DisplayBits(	0x000000000008 );
	this->display_[ 0 ][   D_9 ] = DisplayBits(	0x000000000010 );
	this->display_[ 0 ][ D_LDP ] = DisplayBits(	0x000000000080 );
	this->display_[ 0 ][ D_RDP ] = DisplayBits( 0x000000000040 );

//	0x000000ff0f00
	this->display_[ 1 ][   D_0 ] = DisplayBits(	0x000000000400 );
	this->display_[ 1 ][   D_1 ] = DisplayBits(	0x000000020000 );
	this->display_[ 1 ][   D_2 ] = DisplayBits(	0x000000010000 );
	this->display_[ 1 ][   D_3 ] = DisplayBits(	0x000000040000 );
	this->display_[ 1 ][   D_4 ] = DisplayBits(	0x000000080000 );
	this->display_[ 1 ][   D_5 ] = DisplayBits(	0x000000800000 );
	this->display_[ 1 ][   D_6 ] = DisplayBits(	0x000000400000 );
	this->display_[ 1 ][   D_7 ] = DisplayBits(	0x000000200000 );
	this->display_[ 1 ][   D_8 ] = DisplayBits(	0x000000100000 );
	this->display_[ 1 ][   D_9 ] = DisplayBits(	0x000000000800 );
	this->display_[ 1 ][ D_LDP ] = DisplayBits(	0x000000000100 );
	this->display_[ 1 ][ D_RDP ] = DisplayBits( 0x000000000200 );

//	0x00f0ff000000
	this->display_[ 2 ][   D_0 ] = DisplayBits(	0x000020000000 );
	this->display_[ 2 ][   D_1 ] = DisplayBits(	0x004000000000 );
	this->display_[ 2 ][   D_2 ] = DisplayBits(	0x008000000000 );
	this->display_[ 2 ][   D_3 ] = DisplayBits(	0x002000000000 );
	this->display_[ 2 ][   D_4 ] = DisplayBits(	0x001000000000 );
	this->display_[ 2 ][   D_5 ] = DisplayBits(	0x000001000000 );
	this->display_[ 2 ][   D_6 ] = DisplayBits(	0x000002000000 );
	this->display_[ 2 ][   D_7 ] = DisplayBits(	0x000004000000 );
	this->display_[ 2 ][   D_8 ] = DisplayBits(	0x000008000000 );
	this->display_[ 2 ][   D_9 ] = DisplayBits(	0x000010000000 );
	this->display_[ 2 ][ D_LDP ] = DisplayBits(	0x000080000000 );
	this->display_[ 2 ][ D_RDP ] = DisplayBits( 0x000040000000 );

//	0xff0f00000000
	this->display_[ 3 ][   D_0 ] = DisplayBits(	0x000400000000 );
	this->display_[ 3 ][   D_1 ] = DisplayBits(	0x020000000000 );
	this->display_[ 3 ][   D_2 ] = DisplayBits(	0x010000000000 );
	this->display_[ 3 ][   D_3 ] = DisplayBits(	0x040000000000 );
	this->display_[ 3 ][   D_4 ] = DisplayBits(	0x080000000000 );
	this->display_[ 3 ][   D_5 ] = DisplayBits(	0x800000000000 );
	this->display_[ 3 ][   D_6 ] = DisplayBits(	0x400000000000 );
	this->display_[ 3 ][   D_7 ] = DisplayBits(	0x200000000000 );
	this->display_[ 3 ][   D_8 ] = DisplayBits(	0x100000000000 );
	this->display_[ 3 ][   D_9 ] = DisplayBits(	0x000800000000 );
	this->display_[ 3 ][ D_LDP ] = DisplayBits(	0x000100000000 );
	this->display_[ 3 ][ D_RDP ] = DisplayBits( 0x000200000000 );
}

void DisplayDecoder::disable()
{
	std::array< bool, 4 >::iterator i = this->digitStates_.begin();
	std::array< bool, 4 >::const_iterator const END = this->digitStates_.end();
	for ( ; i != END; ++i )
	{
		*i = false;
	}
}

void DisplayDecoder::disableDigit(
	uint8_t const DIGIT )
{
	if ( DIGIT < DisplayDecoder::NUM_DIGITS )
	{
		this->digitStates_[ DIGIT ] = false;
	}
}

void DisplayDecoder::enable()
{
	std::array< bool, 4 >::iterator i = this->digitStates_.begin();
	std::array< bool, 4 >::const_iterator const END = this->digitStates_.end();
	for ( ; i != END; ++i )
	{
		*i = true;
	}
}

void DisplayDecoder::enableDigit(
	uint8_t const DIGIT )
{
	if ( DIGIT < DisplayDecoder::NUM_DIGITS )
	{
		this->digitStates_[ DIGIT ] = true;
	}
}

void DisplayDecoder::setDigit(
	uint8_t const DIGIT,
	uint8_t const VALUE )
{
	// Only assign if the digit is within range, noting that DIGIT paramter
	// is an unsigned integer (i.e., no minimum check is required).
	if ( DIGIT < DisplayDecoder::NUM_DIGITS )
	{
		this->digitValues_[ DIGIT ] = VALUE;
	}
}

void DisplayDecoder::updateDisplay() const
{
	this->bitPattern_.reset();
	for ( uint8_t i = 0; i < DisplayDecoder::NUM_DIGITS; ++i )
	{
		if ( this->digitStates_[ i ] )
		{
			this->convertDigit2Bits( i, this->digitValues_[ i ], this->bitPattern_ );
		}
	}
	this->toggleRegisterBits( this->bitPattern_ );
}

void DisplayDecoder::convertDigit2Bits(
	uint8_t const DIGIT,
	uint8_t const DIGIT_VALUE,
	DisplayBits & bit_pattern ) const
{
	// If the digit is in range then 'or' the bit pattern for its value,
	// otherwise no change.
	if ( DIGIT < DisplayDecoder::NUM_DIGITS )
	{
		bit_pattern |= this->display_[ DIGIT ][ DIGIT_VALUE ];
	}
}

void DisplayDecoder::toggleRegisterBits(
	DisplayBits const & BIT_PATTERN ) const
{
	for ( uint8_t i = 0; i < 48; ++i )
	{
		if ( BIT_PATTERN[ i ] )
		{
			digitalWrite( SER_IN, AL_HIGH );
		}

		digitalWrite( SRCK,		AL_HIGH );
		digitalWrite( SER_IN,	AL_LOW );
		digitalWrite( SRCK,		AL_LOW );
	}

	// Clock the stored values to the outputs.
	digitalWrite( RCK,	AL_HIGH );
	digitalWrite( RCK, 	AL_LOW );
}
