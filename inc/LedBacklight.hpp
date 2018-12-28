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

#if !defined( NIXIE_CLOCK_LED_BACKLIGHT_HPP )
#define NIXIE_CLOCK_LED_BACKLIGHT_HPP

#include <cstdint>

namespace nixie_clock
{

//! \brief ??
class LedBacklight
{
    public:
        //! \brief ??
		static uint8_t const INTENSITY_MAX = 8;

        //! \brief ??
		static uint8_t const INTENSITY_MIN = 0;

        //! \brief ??
		static uint8_t const INTENSITY_STEP = 32;

        //! \brief ??
		static uint16_t const LED_VALUE_MAX = 256;

    public:
        //! \brief ??
        //! \return ??
		static LedBacklight & instance();

    public:
        //! \brief ??
 		LedBacklight( LedBacklight const & ) = delete;

        //! \brief ??
        void operator=( LedBacklight const & ) = delete;

        //! \brief ??
		void decrementBlue();

        //! \brief ??
		void decrementBrightness();

        //! \brief ??
		void decrementGreen();

        //! \brief ??
		void decrementRed();

        //! \brief ??
		void disable();

        //! \brief ??
		void enable();

        //! \brief ??
        //! \return ??
		bool enabled();

        //! \brief ??
        //! \return ??
		uint8_t getBrightness();

        //! \brief ??
        //! \return ??
		uint8_t getBlue();

        //! \brief ??
        //! \return ??
		uint8_t getGreen();

        //! \brief ??
        //! \return ??
		uint8_t getRed();

        //! \brief ??
		void incrementBrightness();

        //! \brief ??
		void incrementBlue();

        //! \brief ??
		void incrementGreen();

        //! \brief ??
		void incrementRed();

        //! \brief ??
		void loadState();

        //! \brief ??
		void saveState();

        //! \brief ??
        //! \param BRIGHTNESS ??
		void setBrightness(
			uint8_t const BRIGHTNESS );

	private:
        //! \brief ??
        //! \param ENABLED ??
        LedBacklight(
			bool const ENABLED = false );

        //! \brief ??
		void updateBlue();

        //! \brief ??
		void updateGreen();

        //! \brief ??
		void updateRed();

	private:
        //! \brief ??
		static uint16_t const FREQUENCY_LED_PWM = 500;

	private:
        //! \brief ??
		uint8_t blue_;

        //! \brief ??
		uint8_t brightness_;

        //! \brief ??
		bool enabled_;

        //! \brief ??
		uint8_t green_;

        //! \brief ??
		uint8_t red_;
};

}

#endif
