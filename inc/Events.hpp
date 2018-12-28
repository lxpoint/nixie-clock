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

#if !defined( NIXIE_CLOCK_EVENTS_HPP )
#define NIXIE_CLOCK_EVENTS_HPP

#include <boost/statechart/event.hpp>

namespace nixie_clock
{

//! \brief ??
struct EvtSecondTick : boost::statechart::event< EvtSecondTick >
{
};

//! \brief ??
struct EvtButton0Long : boost::statechart::event< EvtButton0Long >
{
};

//! \brief ??
struct EvtButton0Short : boost::statechart::event< EvtButton0Short >
{
};

//! \brief ??
struct EvtButton1Long : boost::statechart::event< EvtButton1Long >
{
};

//! \brief ??
struct EvtButton1Short : boost::statechart::event< EvtButton1Short >
{
};

//! \brief ??
struct EvtButton2Long : boost::statechart::event< EvtButton2Long >
{
};

//! \brief ??
struct EvtButton2Short : boost::statechart::event< EvtButton2Short >
{
};

}

#endif
