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
#include "../core/macros.h"
#include "../core/Resource.h"
#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

namespace easy2d
{
	class Transcoder
	{
		WAVEFORMATEX* wave_format_;

	public:
		Transcoder();

		~Transcoder();

		const WAVEFORMATEX* GetWaveFormatEx() const;

		HRESULT LoadMediaFile(
			LPCWSTR file_path,
			BYTE** wave_data,
			UINT32* wave_data_size
		);

		HRESULT LoadMediaResource(
			Resource const& res,
			BYTE** wave_data,
			UINT32* wave_data_size
		);

		HRESULT ReadSource(
			IMFSourceReader* reader,
			BYTE** wave_data,
			UINT32* wave_data_size
		);
	};
}