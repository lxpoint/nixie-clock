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

#if !defined( NIXIE_CLOCK_CLOCK_MODEL_HPP )
#define NIXIE_CLOCK_CLOCK_MODEL_HPP

#include <cstdint>

namespace nixie_clock
{

//! \brief ??
class ClockModel
{
    public:
        //! \brief ??
        //! \return ??
		static ClockModel & instance();

    public:
        //! \brief ??
 		ClockModel( ClockModel const & ) = delete;

        //! \brief ??
        void operator=( ClockModel const & ) = delete;

    public:
        //! \brief ??
        //! \return ??
		uint8_t getDay() const;

        //! \brief ??
        //! \return ??
		uint8_t getHours() const;

        //! \brief ??
        //! \return ??
		uint8_t getMinutes() const;

        //! \brief ??
        //! \return ??
		bool getMode() const;

        //! \brief ??
        //! \return ??
		uint8_t getMonth() const;

        //! \brief ??
        //! \return ??
		uint8_t getSeconds() const;

        //! \brief ??
        //! \return ??
		uint16_t getYear() const;

        //! \brief ??
        //! \param MODE_24_HOUR ??
		void setMode(
			bool const MODE_24_HOUR );

        //! \brief Performs housekeeping functions required on a regular
        //! basis, currently focused on tracking daylight saving time.
		void update() const;

	private:
        //! \brief ??
        //! \param MODE_24_HOUR ??
        ClockModel(
			bool const MODE_24_HOUR = false );

        //! \brief ??
        //! \return ??
		bool isDST() const;

	private:
        //! \brief ??
		bool mode24Hour_;
};

}

#endif
