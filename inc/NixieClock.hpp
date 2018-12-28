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

#if !defined( NIXIE_CLOCK_NIXIE_CLOCK_HPP )
#define NIXIE_CLOCK_NIXIE_CLOCK_HPP

#include "ClockDisplay.hpp"
#include "ClockModel.hpp"
#include "Events.hpp"
#include "LedBacklight.hpp"
#include "TimerModel.hpp"

#include <cstdint>
#include <boost/statechart/history.hpp>
#include <boost/statechart/in_state_reaction.hpp>
#include <boost/statechart/state_machine.hpp>
#include <boost/statechart/simple_state.hpp>
#include <boost/statechart/state.hpp>
#include <boost/statechart/transition.hpp>
#include <boost/statechart/custom_reaction.hpp>
#include <boost/mpl/list.hpp>

namespace nixie_clock
{


static int const DIGIT_DISPLAY_BRIGHTNESS 	= 1;
static int const DIGIT_TIME_MODE		 	= 2;
static int const DIGIT_DATE_MODE		 	= 3;
static int const DIGIT_DISPLAY_RED			= 4;
static int const DIGIT_DISPLAY_GREEN		= 5;
static int const DIGIT_DISPLAY_BLUE 		= 6;


struct TimeDateDisplay;
struct SetAlarm;
struct CountdownTimer;
struct Configuration;

// Time & date display substates.
struct DisplayTime;
struct DisplayDate;
struct BacklightOn;
struct BacklightOff;
struct DisplayEnabled;
struct DisplayDisabled;

// CountdownTimer substates.
struct TimerFinished;
struct TimerRunning;
struct TimerSet;
struct TimerStopped;

// Configuration substates.
struct BrightnessSet;
struct TimeDisplayMode;
struct Display12Hour;
struct Display24Hour;
struct DateDisplayMode;
struct DateModeDayFirst;
struct DateModeMonthFirst;

// Other
struct TimeSetHours;
struct TimeSetMinutes;
struct ColourSetBlue;
struct ColourSetGreen;
struct ColourSetRed;


// State machine.

//! \brief ??
struct NixieClock : boost::statechart::state_machine< NixieClock, TimeDateDisplay >
{
};


//!
//! Top level states.
//!

//! \brief ??
struct TimeDateDisplay : boost::statechart::state< TimeDateDisplay, NixieClock, boost::mpl::list< DisplayTime, BacklightOff, DisplayEnabled > >
{
	public:
		//! \brief ??
		TimeDateDisplay( my_context ctx ) :
			my_base( ctx )
		{
			LedBacklight::instance().loadState();
			if ( LedBacklight::instance().enabled() )
			{
				post_event( EvtButton1Short() );
			}
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Long, SetAlarm >,
			boost::statechart::transition< EvtButton1Long, CountdownTimer >,
			boost::statechart::transition< EvtButton2Long, Configuration > > reactions;
};


//! \brief ??
//!
//! Button 0 long => Return to time/date display.
//! Button 0 short (set timer) => Change digit, starting left and to the right, wrapping.
//! Button 1 short (set timer) => Decrement digit.
//! Button 2 short (set timer) => Increment digit.
struct SetAlarm : boost::statechart::simple_state< SetAlarm, NixieClock >
{
	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Long, TimeDateDisplay > reactions;
};


//! \brief ??
//!
//! Button 0 long => Return to time/date display.
//! Button 1 long => Start/stop timer.
//! Button 2 long => Set timer (including reset and stop if running).
//! Button 0 short (set timer) => Change digit, starting left and to the right, wrapping.
//! Button 1 short (set timer) => Decrement digit.
//! Button 2 short (set timer) => Increment digit.
struct CountdownTimer : boost::statechart::simple_state< CountdownTimer, NixieClock, TimerStopped >
{
	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Long, TimeDateDisplay > reactions;
};


//! \brief ??
//!
//! Button 0 long => Return to time/date display.
//! Button 0 short => Change option, wrapping.
//! Button 1 short => Decrement option value.
//! Button 2 short => Increment option value.
struct Configuration : boost::statechart::simple_state< Configuration, NixieClock, BrightnessSet >
{
	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Long, TimeDateDisplay > reactions;
};



//!
//! TimeDateDisplay orthogonal states.
//!

//! \brief ??
struct DisplayTime : boost::statechart::simple_state<
	DisplayTime, TimeDateDisplay::orthogonal< 0 > >
{
	public:
		//! \brief ??
		DisplayTime()
		{
			ClockDisplay::instance().updateTime( ClockModel::instance() );
		}

		//! \brief ??
		//! \param EVENT ??
		void secondTick(
			EvtSecondTick const & EVENT )
		{
			ClockDisplay::instance().updateTime( ClockModel::instance() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, DisplayDate >,
			boost::statechart::in_state_reaction< EvtSecondTick, DisplayTime, &DisplayTime::secondTick > > reactions;
};

//! \brief ??
struct DisplayDate : boost::statechart::simple_state<
	DisplayDate, TimeDateDisplay::orthogonal< 0 > >
{
	public:
		//! \brief ??
		DisplayDate()
		{
			ClockDisplay::instance().updateDate( ClockModel::instance() );
		}

		//! \brief ??
		//! \param EVENT ??
		void secondTick(
			EvtSecondTick const & EVENT )
		{
			ClockDisplay::instance().updateDate( ClockModel::instance() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, DisplayTime >,
			boost::statechart::in_state_reaction< EvtSecondTick, DisplayDate, &DisplayDate::secondTick > > reactions;
};

//! \brief ??
struct BacklightOff : boost::statechart::simple_state<
	BacklightOff, TimeDateDisplay::orthogonal< 1 > >
{
	public:
		//! \brief ??
		BacklightOff()
		{
			LedBacklight::instance().disable();
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton1Short, BacklightOn > reactions;
};

//! \brief ??
struct BacklightOn : boost::statechart::simple_state<
	BacklightOn, TimeDateDisplay::orthogonal< 1 > >
{
	public:
		//! \brief ??
		BacklightOn()
		{
			LedBacklight::instance().enable();
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton1Short, BacklightOff > reactions;
};

//! \brief ??
struct DisplayDisabled : boost::statechart::simple_state<
	DisplayDisabled, TimeDateDisplay::orthogonal< 2 > >
{
	public:
		//! \brief ??
		DisplayDisabled()
		{
			ClockDisplay::instance().disable();
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton2Short, DisplayEnabled > reactions;
};

//! \brief ??
struct DisplayEnabled : boost::statechart::simple_state<
	DisplayEnabled, TimeDateDisplay::orthogonal< 2 > >
{
	public:
		//! \brief ??
		DisplayEnabled()
		{
			ClockDisplay::instance().enable();
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton2Short, DisplayDisabled > reactions;
};



//!
//! CountdownTimer substates.
//!

//! \brief ??
struct TimerStopped : boost::statechart::simple_state<
	TimerStopped, CountdownTimer >
{
	public:
		//! \brief ??
		TimerStopped()
		{
			TimerModel::instance().stop();
			ClockDisplay::instance().updateTime( TimerModel::instance() );
		}

		//! \brief ??
		void resetTimer(
			EvtButton2Short const & EVENT )
		{
			TimerModel::instance().reset();
			ClockDisplay::instance().updateTime( TimerModel::instance() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton1Short, TimerRunning >,
			boost::statechart::in_state_reaction< EvtButton2Short, TimerStopped, &TimerStopped::resetTimer >,
			boost::statechart::transition< EvtButton2Long, TimerSet > > reactions;
};

//! \brief ??
struct TimerRunning : boost::statechart::simple_state<
	TimerRunning, CountdownTimer >
{
	public:
		//! \brief ??
		TimerRunning()
		{
			TimerModel::instance().start();
		}

		//! \brief ??
		//! \param EVENT ??
		boost::statechart::result react(
			EvtSecondTick const & EVENT )
		{
			TimerModel::instance().update();
			ClockDisplay::instance().updateTime( TimerModel::instance() );
			if ( TimerModel::instance().isFinished() )
			{
				return transit< TimerFinished >();
			}
			else
			{
				return discard_event();
			}
		}

		//! \brief ??
		void resetTimer(
			EvtButton2Short const & EVENT )
		{
			TimerModel::instance().reset();
			ClockDisplay::instance().updateTime( TimerModel::instance() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, TimerStopped >,
			boost::statechart::in_state_reaction< EvtButton2Short, TimerRunning, &TimerRunning::resetTimer >,
			boost::statechart::transition< EvtButton2Long, TimerSet >,
			boost::statechart::custom_reaction< EvtSecondTick > > reactions;
//			boost::statechart::in_state_reaction< EvtSecondTick, TimerRunning, &TimerRunning::secondTick > > reactions;
};

//! \brief ??
struct TimerSet : boost::statechart::simple_state<
	TimerSet, CountdownTimer >
{
	public:
		//! \brief ??
		TimerSet() :
			digitGroup_( TimerSet::DIGITS_HOURS )
		{
//			TimerModel::instance().reset();
			ClockDisplay::instance().flashHours( true );
			ClockDisplay::instance().flashEnable();
		}

		//! \brief ??
		~TimerSet()
		{
			ClockDisplay::instance().flashDisable();
			ClockDisplay::instance().flashHours( false );
			ClockDisplay::instance().flashMinutes( false );
		}

		//! \brief ??
		void changeDigit(
			EvtButton0Short const & EVENT )
		{
			ClockDisplay::instance().flashDisable();

			switch ( this->digitGroup_ )
			{
				case TimerSet::DIGITS_HOURS:
				{
					ClockDisplay::instance().flashHours( false );
					ClockDisplay::instance().flashMinutes( true );
					break;
				}
				case TimerSet::DIGITS_MINUTES:
				{
					ClockDisplay::instance().flashHours( true );
					ClockDisplay::instance().flashMinutes( false );
					break;
				}
			}

			ClockDisplay::instance().flashEnable();
			this->digitGroup_ += 1;
			this->digitGroup_ %= TimerSet::DIGIT_GROUPS;
		}

		//! \brief ??
		void decrementDigit(
			EvtButton1Short const & EVENT )
		{
			if ( this->digitGroup_ == DIGITS_HOURS )
			{
				TimerModel::instance().decrementHours();
			}
			else
			{
				TimerModel::instance().decrementMinutes();
			}
			ClockDisplay::instance().updateTime( TimerModel::instance() );
		}

		//! \brief ??
		void incrementDigit(
			EvtButton2Short const & EVENT )
		{
			if ( this->digitGroup_ == DIGITS_HOURS )
			{
				TimerModel::instance().incrementHours();
			}
			else
			{
				TimerModel::instance().incrementMinutes();
			}
			ClockDisplay::instance().updateTime( TimerModel::instance() );
		}

	public:
		//! \brief ??
		static uint8_t const DIGITS_HOURS = 0;

		//! \brief ??
		static uint8_t const DIGITS_MINUTES = 1;

		//! \brief ??
		static uint8_t const DIGITS_SECONDS = 2;

		//! \brief ??
		static uint8_t const DIGIT_GROUPS = 2;

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton2Long, TimerStopped >,
			boost::statechart::in_state_reaction< EvtButton0Short, TimerSet, &TimerSet::changeDigit >,
			boost::statechart::in_state_reaction< EvtButton1Short, TimerSet, &TimerSet::decrementDigit >,
			boost::statechart::in_state_reaction< EvtButton2Short, TimerSet, &TimerSet::incrementDigit > > reactions;

	private:
		//! \brief ??
		uint8_t digitGroup_;
};

//! \brief ??
struct TimerFinished : boost::statechart::simple_state<
	TimerFinished, CountdownTimer >
{
	public:
		//! \brief ??
		TimerFinished()
		{
			ClockDisplay::instance().flashHours( true );
			ClockDisplay::instance().flashMinutes( true );
			ClockDisplay::instance().flashEnable();
		}

		//! \brief ??
		~TimerFinished()
		{
			ClockDisplay::instance().flashDisable();
			ClockDisplay::instance().flashHours( false );
			ClockDisplay::instance().flashMinutes( false );
			TimerModel::instance().reset();
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, TimerStopped >,
			boost::statechart::transition< EvtButton2Long, TimerSet > > reactions;
};


//!
//! Configuration substates.
//!

//! \brief ??
struct BrightnessSet : boost::statechart::simple_state<
	BrightnessSet, Configuration >
{
	public:
		//! \brief ??
		BrightnessSet() :
			backlitOnEntry_( LedBacklight::instance().enabled() )
		{
			// Enable the backlight.
			LedBacklight::instance().enable();

			// Display the configuration options for backlight set.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BRIGHTNESS, LedBacklight::instance().getBrightness() );
		}

		//! \brief ??
		~BrightnessSet()
		{
			// Disable the backlight if it wasn't on when entering backlight set.
			if ( !this->backlitOnEntry_ )
			{
				LedBacklight::instance().disable();
			}

			// Disable the configuration option for backlight set.
			ClockDisplay::instance().disableOptionDisplay();
		}

		//! \brief ??
		void decreaseIntensity(
			EvtButton1Short const & EVENT )
		{
			// Decrease the brightness by one.
			LedBacklight::instance().decrementBrightness();

			// Update the display to reflect the new setting.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BRIGHTNESS, LedBacklight::instance().getBrightness() );
		}

		void increaseIntensity(
			EvtButton2Short const & EVENT )
		{
			// Increase the brightness by one.
			LedBacklight::instance().incrementBrightness();

			// Update the display to reflect the new setting.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BRIGHTNESS, LedBacklight::instance().getBrightness() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, TimeDisplayMode >,
			boost::statechart::in_state_reaction< EvtButton1Short, BrightnessSet, &BrightnessSet::decreaseIntensity >,
			boost::statechart::in_state_reaction< EvtButton2Short, BrightnessSet, &BrightnessSet::increaseIntensity > > reactions;

	private:
		//! \brief ??
		bool backlitOnEntry_;
};

//! \brief ??
struct TimeDisplayMode : boost::statechart::state<
	TimeDisplayMode, Configuration, Display12Hour >
{
	public:
		//! \brief ??
		TimeDisplayMode( my_context ctx ) :
			my_base( ctx )
		{
			if ( ClockModel::instance().getMode() )
			{
				post_event( EvtButton2Short() );
			}
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Short, DateDisplayMode > reactions;
};

//! \brief ??
struct Display12Hour : boost::statechart::simple_state<
	Display12Hour, TimeDisplayMode >
{
	public:
		//! \brief ??
		Display12Hour()
		{
			ClockDisplay::instance().enableOptionDisplay( DIGIT_TIME_MODE, 12 );
			ClockModel::instance().setMode( false );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton1Short, Display24Hour >,
			boost::statechart::transition< EvtButton2Short, Display24Hour > > reactions;
};

//! \brief ??
struct Display24Hour : boost::statechart::simple_state<
	Display24Hour, TimeDisplayMode >
{
	public:
		//! \brief ??
		Display24Hour()
		{
			ClockDisplay::instance().enableOptionDisplay( DIGIT_TIME_MODE, 24 );
			ClockModel::instance().setMode( true );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton1Short, Display12Hour >,
			boost::statechart::transition< EvtButton2Short, Display12Hour > > reactions;
};

//! \brief ??
struct DateDisplayMode : boost::statechart::state<
	DateDisplayMode, Configuration, DateModeDayFirst >
{
	public:
		//! \brief ??
		DateDisplayMode( my_context ctx ) :
			my_base( ctx )
		{
			if ( !ClockDisplay::instance().getDateMode() )
			{
				post_event( EvtButton2Short() );
			}
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Short, ColourSetRed > reactions;
};

//! \brief ??
struct DateModeDayFirst : boost::statechart::simple_state<
	DateModeDayFirst, DateDisplayMode >
{
	public:
		//! \brief ??
		DateModeDayFirst()
		{
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DATE_MODE, 1 );
			ClockDisplay::instance().setDateMode( true );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton1Short, DateModeMonthFirst >,
			boost::statechart::transition< EvtButton2Short, DateModeMonthFirst > > reactions;
};

//! \brief ??
struct DateModeMonthFirst : boost::statechart::simple_state<
	DateModeMonthFirst, DateDisplayMode >
{
	public:
		//! \brief ??
		DateModeMonthFirst()
		{
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DATE_MODE, 2 );
			ClockDisplay::instance().setDateMode( false );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton1Short, DateModeDayFirst >,
			boost::statechart::transition< EvtButton2Short, DateModeDayFirst > > reactions;
};

//! \brief ??
struct ColourSetRed : boost::statechart::simple_state< ColourSetRed, Configuration >
{
	public:
		//! \brief ??
		ColourSetRed() :
			backlitOnEntry_( LedBacklight::instance().enabled() )
		{
			// Enable the backlight.
			LedBacklight::instance().enable();

			// Display the configuration options for backlight set.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_RED, LedBacklight::instance().getRed() );
		}

		//! \brief ??
		~ColourSetRed()
		{
			// Disable the backlight if it wasn't on when entering backlight set.
			if ( !this->backlitOnEntry_ )
			{
				LedBacklight::instance().disable();
			}

			// Disable the configuration option for backlight set.
			ClockDisplay::instance().disableOptionDisplay();
		}

		void decreaseColour(
			EvtButton1Short const & EVENT )
		{
			LedBacklight::instance().decrementRed();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_RED, LedBacklight::instance().getRed() );
		}

		void increaseColour(
			EvtButton2Short const & EVENT )
		{
			LedBacklight::instance().incrementRed();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_RED, LedBacklight::instance().getRed() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, ColourSetGreen >,
			boost::statechart::in_state_reaction< EvtButton1Short, ColourSetRed, &ColourSetRed::decreaseColour >,
			boost::statechart::in_state_reaction< EvtButton2Short, ColourSetRed, &ColourSetRed::increaseColour > > reactions;

	private:
		//! \brief ??
		bool backlitOnEntry_;
};

//! \brief ??
struct ColourSetGreen : boost::statechart::simple_state< ColourSetGreen, Configuration >
{
	public:
		//! \brief ??
		ColourSetGreen() :
			backlitOnEntry_( LedBacklight::instance().enabled() )
		{
			// Enable the backlight.
			LedBacklight::instance().enable();

			// Display the configuration options for backlight set.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_GREEN, LedBacklight::instance().getGreen() );
		}

		//! \brief ??
		~ColourSetGreen()
		{
			// Disable the backlight if it wasn't on when entering backlight set.
			if ( !this->backlitOnEntry_ )
			{
				LedBacklight::instance().disable();
			}

			// Disable the configuration option for backlight set.
			ClockDisplay::instance().disableOptionDisplay();
		}

		void decreaseColour(
			EvtButton1Short const & EVENT )
		{
			LedBacklight::instance().decrementGreen();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_GREEN, LedBacklight::instance().getGreen() );
		}

		void increaseColour(
			EvtButton2Short const & EVENT )
		{
			LedBacklight::instance().incrementGreen();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_GREEN, LedBacklight::instance().getGreen() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, ColourSetBlue >,
			boost::statechart::in_state_reaction< EvtButton1Short, ColourSetGreen, &ColourSetGreen::decreaseColour >,
			boost::statechart::in_state_reaction< EvtButton2Short, ColourSetGreen, &ColourSetGreen::increaseColour > > reactions;

	private:
		//! \brief ??
		bool backlitOnEntry_;
};

//! \brief ??
struct ColourSetBlue : boost::statechart::simple_state< ColourSetBlue, Configuration >
{
	public:
		//! \brief ??
		ColourSetBlue() :
			backlitOnEntry_( LedBacklight::instance().enabled() )
		{
			// Enable the backlight.
			LedBacklight::instance().enable();

			// Display the configuration options for backlight set.
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BLUE, LedBacklight::instance().getBlue() );
		}

		//! \brief ??
		~ColourSetBlue()
		{
			// Disable the backlight if it wasn't on when entering backlight set.
			if ( !this->backlitOnEntry_ )
			{
				LedBacklight::instance().disable();
			}

			// Disable the configuration option for backlight set.
			ClockDisplay::instance().disableOptionDisplay();
		}

		void decreaseColour(
			EvtButton1Short const & EVENT )
		{
			LedBacklight::instance().decrementBlue();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BLUE, LedBacklight::instance().getBlue() );
		}

		void increaseColour(
			EvtButton2Short const & EVENT )
		{
			LedBacklight::instance().incrementBlue();
			ClockDisplay::instance().enableOptionDisplay( DIGIT_DISPLAY_BLUE, LedBacklight::instance().getBlue() );
		}

	public:
		//! \brief ??
		typedef boost::mpl::list<
			boost::statechart::transition< EvtButton0Short, BrightnessSet >,
			boost::statechart::in_state_reaction< EvtButton1Short, ColourSetBlue, &ColourSetBlue::decreaseColour >,
			boost::statechart::in_state_reaction< EvtButton2Short, ColourSetBlue, &ColourSetBlue::increaseColour > > reactions;

	private:
		//! \brief ??
		bool backlitOnEntry_;
};


/*
//! \brief ??
struct TimeSetHours : boost::statechart::simple_state< TimeSetHours, Configuration >
{
	public:
		//! \brief ??
		TimeSetHours()
		{
			ClockDisplay::instance().update( ClockModel::instance() );
			ClockDisplay::instance().flashHours( true );
		}

		//! \brief ??
		~TimeSetHours()
		{
			ClockDisplay::instance().flashHours( false );
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Short, TimeSetMinutes > reactions;
};


//! \brief ??
struct TimeSetMinutes : boost::statechart::simple_state< TimeSetMinutes, Configuration >
{
	public:
		//! \brief ??
		TimeSetMinutes()
		{
			ClockDisplay::instance().update( ClockModel::instance() );
			ClockDisplay::instance().flashMinutes( true );
		}

		//! \brief ??
		~TimeSetMinutes()
		{
			ClockDisplay::instance().flashMinutes( false );
		}

	public:
		//! \brief ??
		typedef boost::statechart::transition< EvtButton0Short, BrightnessSet > reactions;
};
*/


}

#endif
