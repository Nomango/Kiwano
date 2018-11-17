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
#include "logs.h"
#include <ctime>
#include <iomanip>

namespace easy2d
{
	namespace logs
	{
		namespace
		{
			bool enabled = true;

			void Out(std::ostream& stream, const char* output)
			{
				stream << output;
				::OutputDebugStringA(output);
			}

			void OutPrefix(std::stringstream& ss)
			{
				std::time_t unix = ::time(NULL);
				struct tm tmbuf;
				localtime_s(&tmbuf, &unix);
				ss << std::put_time(&tmbuf, "[easy2d] %H:%M:%S ");
			}

			void Output(std::ostream& stream, const char* prompt, const char* format, va_list args)
			{
				if (!enabled)
					return;

				size_t len = ::_vscprintf(format, args) + 1;
				char* buffer = new char[len];
				::_vsnprintf_s(buffer, len, len, format, args);

				std::stringstream ss;
				OutPrefix(ss);
				ss << prompt << buffer;
				Out(stream, ss.str().c_str());

				delete[] buffer;
			}

			void OutputLine(std::ostream& stream, const char* prompt, const char* format, va_list args)
			{
				if (!enabled)
					return;

				Output(stream, prompt, format, args);
				Out(stream, "\r\n");
			}
		}

		void easy2d::logs::Enable()
		{
			enabled = true;
		}

		void easy2d::logs::Disable()
		{
			enabled = false;
		}

		void Print(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::cout, "", format, args);

			va_end(args);
		}

		void Println(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::cout, "", format, args);

			va_end(args);
		}

		void Warning(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::cerr, "Warning: ", format, args);

			va_end(args);
		}

		void Warningln(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::cerr, "Warning: ", format, args);

			va_end(args);
		}

		void Error(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			Output(std::cerr, "Error: ", format, args);

			va_end(args);
		}

		void Errorln(const char* format, ...)
		{
			va_list args = nullptr;
			va_start(args, format);

			OutputLine(std::cerr, "Error: ", format, args);

			va_end(args);
		}

		void Errorln(HRESULT hr)
		{
			Errorln("failure with HRESULT of %08X", hr);
		}

		void Errorln(HRESULT hr, const char* output)
		{
			Errorln("failure with HRESULT of %08X: %s", hr, output);
		}
	}
}
