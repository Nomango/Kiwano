// Copyright (c) 2018-2019 Kiwano - Nomango
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

#include <kiwano-physics/ContactEvent.h>

namespace kiwano
{
	namespace physics
	{
		ContactBeginEvent::ContactBeginEvent()
			: Event(KGE_EVENT(ContactBeginEvent))
			, body_a(nullptr)
			, body_b(nullptr)
		{
		}

		ContactBeginEvent::ContactBeginEvent(Contact const& contact)
			: ContactBeginEvent()
		{
			this->contact = contact;
			body_a = this->contact.GetFixtureA().GetBody();
			body_b = this->contact.GetFixtureB().GetBody();
		}

		ContactEndEvent::ContactEndEvent()
			: Event(KGE_EVENT(ContactEndEvent))
			, body_a(nullptr)
			, body_b(nullptr)
		{
		}

		ContactEndEvent::ContactEndEvent(Contact const& contact)
			: ContactEndEvent()
		{
			this->contact = contact;
			body_a = this->contact.GetFixtureA().GetBody();
			body_b = this->contact.GetFixtureB().GetBody();
		}

}
}