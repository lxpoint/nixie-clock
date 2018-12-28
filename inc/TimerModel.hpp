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

#if !defined( NIXIE_CLOCK_TIMER_MODEL_HPP )
#define NIXIE_CLOCK_TIMER_MODEL_HPP

#include <cstdint>

namespace nixie_clock
{

//! \brief ??
class TimerModel
{
    public:
        //! \brief ??
        //! \return ??
		static TimerModel & instance();

    public:
        //! \brief ??
 		TimerModel( TimerModel const & ) = delete;

        //! \brief ??
        void operator=( TimerModel const & ) = delete;

    public:
        //! \brief ??
		void decrementHours();

        //! \brief ??
		void decrementMinutes();

        //! \brief ??
		void decrementSeconds();

        //! \brief ??
        //! \return ??
		uint8_t getHours() const;

        //! \brief ??
        //! \return ??
		uint8_t getMinutes() const;

        //! \brief ??
        //! \return ??
		uint8_t getSeconds() const;

        //! \brief ??
		void incrementHours();

        //! \brief ??
		void incrementMinutes();

        //! \brief ??
		void incrementSeconds();

        //! \brief ??
        //! \return ??
		bool isFinished() const;

        //! \brief ??
        //! \return ??
		bool isRunning() const;

        //! \brief ??
		void reset();

        //! \brief ??
		void start();

        //! \brief ??
		void stop();

        //! \brief ??
		void update();

	private:
        //! \brief ??
        TimerModel();

	private:
        //! \brief In seconds.
		unsigned long currentTime_;

        //! \brief ??
		bool finished_;

        //! \brief ??
		uint8_t hours_;

        //! \brief ??
		uint8_t minutes_;

        //! \brief ??
		bool running_;

        //! \brief ??
		uint8_t seconds_;

        //! \brief ??
		uint8_t runningHours_;

        //! \brief ??
		uint8_t runningMinutes_;

        //! \brief ??
		uint8_t runningSeconds_;

        //! \brief In seconds.
		unsigned long startTime_;
};

}

#endif
