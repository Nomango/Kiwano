// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#pragma once
#include "EventListener.h"

namespace easy2d
{
	EventListener::EventListener(EventType type, EventCallback const & callback, String const & name)
		: type_(type)
		, callback_(callback)
		, name_(name)
		, running_(true)
	{
	}

	EventListener::~EventListener()
	{
	}

	void EventListener::Start()
	{
		running_ = true;
	}

	void EventListener::Stop()
	{
		running_ = false;
	}

	bool EventListener::IsRunning() const
	{
		return running_;
	}

	String const & EventListener::GetName() const
	{
		return name_;
	}

	void EventListener::SetName(String const & name)
	{
		name_ = name;
	}

}