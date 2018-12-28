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

#if !defined( NIXIE_CLOCK_CLOCK_DISPLAY_HPP )
#define NIXIE_CLOCK_CLOCK_DISPLAY_HPP

#include "ClockModel.hpp"
#include "DisplayDecoder.hpp"
#include "TimerModel.hpp"

#include <array>
#include <boost/tuple/tuple.hpp>
#include <cstdint>

namespace nixie_clock
{

//! \brief ??
class ClockDisplay
{
	public:
		//! \brief ??
		typedef std::array< bool, 4 > StateArray;

    public:
        //! \brief ??
        //! \return ??
		static ClockDisplay & instance();

    public:
        //! \brief ??
 		ClockDisplay( ClockDisplay const & ) = delete;

        //! \brief ??
        void operator=( ClockDisplay const & ) = delete;

        //! \brief ??
		void disable();

        //! \brief ??
		void disableOptionDisplay();

        //! \brief ??
		void enable();

        //! \brief ??
        //! \param OPTION ??
        //! \param VALUE ??
		void enableOptionDisplay(
			uint8_t const OPTION,
			uint8_t const VALUE );

        //! \brief ??
		void flashDisable();

        //! \brief ??
		void flashEnable();

        //! \brief ??
        //! \param DIGIT ??
        //! \param ENABLED ??
		void flashDigit(
			uint8_t const DIGIT,
			bool const ENABLED );

        //! \brief ??
        //! \param ENABLED ??
		void flashHours(
			bool const ENABLED );

        //! \brief ??
        //! \param ENABLED ??
		void flashMinutes(
			bool const ENABLED );

        //! \brief ??
        //! \return ??
		bool getDateMode();

        //! \brief ??
        //! \param MODE_DAY_FIRST ??
		void setDateMode(
			bool const MODE_DAY_FIRST );

        //! \brief ??
		void toggleDigits();

        //! \brief ??
        //! \param CLOCK_MODEL ??
		void updateDate(
			ClockModel const & CLOCK_MODEL );

        //! \brief ??
        //! \param CLOCK_MODEL ??
		void updateTime(
			ClockModel const & CLOCK_MODEL );

        //! \brief ??
        //! \param CLOCK_MODEL ??
		void updateTime(
			TimerModel const & TIMER_MODEL );

	public:
        //! \brief ??
		static uint8_t const NUMBER_DIGITS = 4;

	private:
        //! \brief ??
		typedef boost::tuple< uint8_t, uint8_t > DigitValues;

	private:
        //! \brief ??
        //! \param ENABLED ??
        ClockDisplay(
			bool const ENABLED = true );

        //! \brief ??
		DigitValues decomposeDigits(
			uint8_t const VALUE );

	private:
        //! \brief ??
		bool dateMode_;

        //! \brief ??
		StateArray digitFlash_;

        //! \brief ??
		DisplayDecoder display_;

        //! \brief ??
		bool flashState_;
};

}

#endif
