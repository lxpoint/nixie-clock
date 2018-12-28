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

#include "application.h"
#include "system.hpp"
#include "Buttons.hpp"
#include "NixieClock.hpp"

using namespace nixie_clock;

// Timer for updating the current time every hour.
uint32_t const TIME_HOUR_MS = 60 * 60 * 1000;
Timer timeUpdateTimer( TIME_HOUR_MS, callbackUpdateTime );

// Timer for updating the clock model every second.
uint32_t const TIME_SECOND_MS = 1000;
Timer secondTimer( TIME_SECOND_MS, callbackSecondTick );

bool buttonHeld = false;
uint8_t buttonPressed = NO_KEY;
uint8_t buttonValue = NO_KEY;
bool flagSecondTick = false;
bool flagUpdateTime = false;
NixieClock nixieClock;

void callbackSecondTick()
{
	flagSecondTick = true;
}

void callbackUpdateTime()
{
	flagUpdateTime = true;
}

void loop()
{
	if ( flagSecondTick )
	{
//Particle.publish( "WiFi:", String( WiFi.RSSI() ), PRIVATE );
		ClockModel::instance().update();
		nixieClock.process_event( EvtSecondTick() );
		flagSecondTick = false;
	}

	if ( flagUpdateTime )
	{
		Particle.syncTime();
		flagUpdateTime = false;
	}

	processButtons();
}

void processButtons()
{
	uint8_t buttonValue = buttonGroup.getKey();
	if ( buttonGroup.get_status() == buttons_pressed )
	{
		buttonPressed = buttonValue;
	}

	if ( buttonGroup.get_status() == buttons_released )
	{
		if ( buttonHeld )
		{
			buttonHeld = false;
		}
		else
		{
			switch( buttonPressed )
			{
				case BUTTON_VALUE_0:
				{
					// Time display state:
					// 		Change backlight mode: on vs. off.
					// Configuration state:
					//
					nixieClock.process_event( EvtButton0Short() );
					break;
				}
				case BUTTON_VALUE_1:
				{
					// Time display state:
					// 		Change time mode: 12 vs. 24 hours.
					// Configuration state:
					//
					nixieClock.process_event( EvtButton1Short() );
					break;
				}
				case BUTTON_VALUE_2:
				{
					// Time display state:
					// 		Change display mode: on vs. off.
					// Configuration state:
					//
					nixieClock.process_event( EvtButton2Short() );
					break;
				}
			}
		}
	}

	if ( ( buttonGroup.get_status() == buttons_held ) &&
		( !buttonHeld ) )
	{
		buttonHeld = true;
		switch( buttonPressed )
		{
			case BUTTON_VALUE_0:
			{
				// Enter configuration state.
				nixieClock.process_event( EvtButton0Long() );
				break;
			}
			case BUTTON_VALUE_1:
			{
				// Enter configuration state.
				nixieClock.process_event( EvtButton1Long() );
				break;
			}
			case BUTTON_VALUE_2:
			{
				// Enter configuration state.
				nixieClock.process_event( EvtButton2Long() );
				break;
			}
		}
	}
}

void setup()
{
	// Enable the buttons.
	initialiseButtons();

	// Set up the clock state machine.
	nixieClock.initiate();

	// Start the timers.
	timeUpdateTimer.start();
	secondTimer.start();
}
