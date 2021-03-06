// Copyright (c) 2016-2018 Kiwano - Nomango
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
#include <kiwano/math/Scalar.h>

namespace kiwano
{
namespace math
{
template <typename _Ty>
struct Vec2T
{
    using ValueType = _Ty;

    ValueType x;
    ValueType y;

    Vec2T()
        : x(ValueType(0))
        , y(ValueType(0))
    {
    }

    Vec2T(ValueType x, ValueType y)
        : x(x)
        , y(y)
    {
    }

    Vec2T(const Vec2T& other)
        : x(other.x)
        , y(other.y)
    {
    }

    inline ValueType Length() const
    {
        return static_cast<ValueType>(math::Sqrt(static_cast<float>(x * x + y * y)));
    }

    inline bool IsOrigin() const
    {
        return (x == 0) && (y == 0);
    }

    inline void Set(ValueType x, ValueType y)
    {
        this->x = x;
        this->y = y;
    }

    inline const Vec2T operator+(const Vec2T& other) const
    {
        return Vec2T(x + other.x, y + other.y);
    }

    inline const Vec2T operator-(const Vec2T& other) const
    {
        return Vec2T(x - other.x, y - other.y);
    }

    inline const Vec2T operator*(ValueType val) const
    {
        return Vec2T(x * val, y * val);
    }

    inline const Vec2T operator/(ValueType val) const
    {
        return Vec2T(x / val, y / val);
    }

    inline const Vec2T operator-() const
    {
        return Vec2T(-x, -y);
    }

    inline Vec2T& operator+=(const Vec2T& other)
    {
        x += other.x;
        y += other.y;
        return (*this);
    }

    inline Vec2T& operator-=(const Vec2T& other)
    {
        x -= other.x;
        y -= other.y;
        return (*this);
    }

    inline Vec2T& operator*=(ValueType val)
    {
        x *= val;
        y *= val;
        return (*this);
    }

    inline Vec2T& operator/=(ValueType val)
    {
        x /= val;
        y /= val;
        return (*this);
    }

    inline bool operator==(const Vec2T& other) const
    {
        return (x == other.x) && (y == other.y);
    }

    inline bool operator!=(const Vec2T& other) const
    {
        return (x != other.x) || (y != other.y);
    }
};
}  // namespace math
}  // namespace kiwano
