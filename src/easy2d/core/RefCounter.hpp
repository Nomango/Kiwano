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
#include "../macros.h"
#include "../common/noncopyable.hpp"

namespace easy2d
{
	class E2D_API RefCounter
		: protected Noncopyable
	{
	public:
		// �������ü���
		inline void Retain() { ++ref_count_; }

		// �������ü���
		inline void Release()
		{
			if (--ref_count_ <= 0)
				delete this;
		}

		// ��ȡ���ü���
		inline long GetRefCount() const { return ref_count_; }

	protected:
		RefCounter() : ref_count_(0) {}

		virtual ~RefCounter() {}

	protected:
		long ref_count_;
	};

	inline void IntrusivePtrAddRef(RefCounter* ptr)
	{
		if (ptr) ptr->Retain();
	}

	inline void IntrusivePtrRelease(RefCounter* ptr)
	{
		if (ptr) ptr->Release();
	}
}