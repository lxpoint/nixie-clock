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

#include "TimerModel.hpp"
#include "application.h"

using namespace nixie_clock;

TimerModel::TimerModel() :
	finished_( false ),
	hours_( 0 ),
	minutes_( 0 ),
	running_( false ),
	seconds_( 10 )
{
}

TimerModel & TimerModel::instance()
{
	static TimerModel instance;
	return instance;
}

void TimerModel::decrementHours()
{
	if ( this->hours_ > 0 )
	{
		this->hours_ -= 1;
	}
}

void TimerModel::decrementMinutes()
{
	if ( this->minutes_ > 0 )
	{
		this->minutes_ -= 1;
	}
}

void TimerModel::decrementSeconds()
{
	if ( this->seconds_ > 0 )
	{
		this->seconds_ -= 1;
	}
}

uint8_t TimerModel::getHours() const
{
	if ( this->running_ )
	{
		return this->runningHours_;
	}
	else
	{
		return this->hours_;
	}
}

uint8_t TimerModel::getMinutes() const
{
	if ( this->running_ )
	{
		return this->runningMinutes_;
	}
	else
	{
		return this->minutes_;
	}
}

uint8_t TimerModel::getSeconds() const
{
	if ( this->running_ )
	{
		return this->runningSeconds_;
	}
	else
	{
		return this->seconds_;
	}
}

void TimerModel::incrementHours()
{
	if ( this->hours_ < 99 )
	{
		this->hours_ += 1;
	}
}

void TimerModel::incrementMinutes()
{
	if ( this->minutes_ < 60 )
	{
		this->minutes_ += 1;
	}
}

void TimerModel::incrementSeconds()
{
	if ( this->seconds_ < 60 )
	{
		this->seconds_ += 1;
	}
}

bool TimerModel::isFinished() const
{
	return this->finished_;
}

bool TimerModel::isRunning() const
{
	return this->running_;
}

void TimerModel::reset()
{
	this->runningHours_ = this->hours_;
	this->runningMinutes_ = this->minutes_;
	this->runningSeconds_ = this->seconds_;
}

void TimerModel::start()
{
	this->running_ = true;
	this->finished_ = false;
}

void TimerModel::stop()
{
	this->running_ = false;
}

void TimerModel::update()
{
	if ( this->runningSeconds_ > 0 )
	{
		this->runningSeconds_ -= 1;
	}
	else
	{
		this->runningSeconds_ = 59;
		if ( this->runningMinutes_ > 0 )
		{
			this->runningMinutes_ -= 1;
		}
		else
		{
			this->runningMinutes_ = 59;
			this->runningHours_ -= 1;
		}
	}

	if ( ( this->runningSeconds_ == 0 ) &&
		( this->runningMinutes_ == 0 ) &&
		( this->runningHours_ == 0 ) )
	{
		this->finished_ = true;
	}
}
