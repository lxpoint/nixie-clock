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

#if !defined( NIXIE_CLOCK_DISPLAY_DECODER_HPP )
#define NIXIE_CLOCK_DISPLAY_DECODER_HPP

#undef max
#undef min
#include <array>
#include <bitset>
#include <cstdint>

namespace nixie_clock
{

//! \brief ??
class DisplayDecoder
{
	public:
        //! \brief ??
		typedef std::bitset< 48 > DisplayBits;

        //! \brief ??
		typedef std::array< DisplayBits, 12 > DisplayBitArray;

        //! \brief ??
		typedef std::array< DisplayBitArray, 4 > DisplayArray;

    public:
        //! \brief ??
        //! \param ENABLE_STATE ??
        DisplayDecoder(
			bool const ENABLE_STATE = false );

        //! \brief ??
		void disable();

        //! \brief ??
        //! \param DIGIT ??
		void disableDigit(
			uint8_t const DIGIT );

        //! \brief ??
		void enable();

        //! \brief ??
        //! \param DIGIT ??
		void enableDigit(
			uint8_t const DIGIT );

        //! \brief ??
        //! \param DIGIT ??
        //! \param VALUE ??
		void setDigit(
			uint8_t const DIGIT,
			uint8_t const VALUE );

        //! \brief ??
		void updateDisplay() const;

	public:
        //! \brief ??
		static uint8_t const NUM_DIGITS = 4;

    private:
        //! \brief ??
        //! \param DIGIT ??
        //! \param DIGIT_VALUE ??
        //! \param bit_pattern ??
		void convertDigit2Bits(
			uint8_t const DIGIT,
			uint8_t const DIGIT_VALUE,
			DisplayBits & bit_pattern ) const;

        //! \brief ??
		void toggleRegisterBits(
			DisplayBits const & BIT_PATTERN ) const;

    private:
        //! \brief ??
		mutable DisplayBits bitPattern_;

        //! \brief ??
        std::array< bool, 4 > digitStates_;

        //! \brief ??
        std::array< uint8_t, 4 > digitValues_;

        //! \brief ??
		DisplayArray display_;

        //! \brief ??
		bool enabled_;
};

}

#endif
