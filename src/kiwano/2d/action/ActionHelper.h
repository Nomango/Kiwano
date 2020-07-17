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
#include <kiwano/2d/action/ActionDelay.h>
#include <kiwano/2d/action/ActionGroup.h>
#include <kiwano/2d/action/ActionTween.h>
#include <kiwano/2d/action/ActionWalk.h>
#include <kiwano/2d/action/Animation.h>

namespace kiwano
{
/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 动画辅助类
struct ActionHelper
{
    /// \~chinese
    /// @brief 设置循环次数
    inline ActionHelper& Loops(int loops)
    {
        ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline ActionHelper& Delay(Duration delay)
    {
        ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline ActionHelper& DoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    inline ActionHelper& LoopDoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline ActionHelper& RemoveTargetWhenDone()
    {
        ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline ActionHelper& Name(const String& name)
    {
        ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 获取指针
    inline ActionPtr Get() const
    {
        return ptr;
    }

    inline ActionHelper(ActionPtr ptr)
        : ptr(ptr)
    {
    }

    inline operator ActionPtr() const
    {
        return ptr;
    }

    inline ActionPtr operator->() const
    {
        return ptr;
    }

private:
    ActionPtr ptr;
};

/// \~chinese
/// @brief 补间动画辅助类
struct TweenHelper
{
    /// \~chinese
    /// @brief 设置动画持续时长
    inline TweenHelper& Dur(Duration dur)
    {
        ptr->SetDuration(dur);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置循环次数
    inline TweenHelper& Loops(int loops)
    {
        ptr->SetLoops(loops);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置缓动函数
    inline TweenHelper& EaseFunc(EaseFunc ease)
    {
        ptr->SetEaseFunc(ease);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画延迟
    inline TweenHelper& Delay(Duration delay)
    {
        ptr->SetDelay(delay);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画结束回调函数
    inline TweenHelper& DoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 设置动画循环结束时的回调函数
    inline TweenHelper& LoopDoneCallback(const Action::DoneCallback& cb)
    {
        ptr->SetLoopDoneCallback(cb);
        return (*this);
    }

    /// \~chinese
    /// @brief 动画结束时移除目标角色
    inline TweenHelper& RemoveTargetWhenDone()
    {
        ptr->RemoveTargetWhenDone();
        return (*this);
    }

    /// \~chinese
    /// @brief 设置名称
    inline TweenHelper& Name(const String& name)
    {
        ptr->SetName(name);
        return (*this);
    }

    /// \~chinese
    /// @brief 获取指针
    inline ActionTweenPtr Get() const
    {
        return ptr;
    }

    inline TweenHelper(ActionTweenPtr ptr)
        : ptr(ptr)
    {
    }

    inline operator ActionPtr() const
    {
        return ptr;
    }

    inline operator ActionTweenPtr() const
    {
        return ptr;
    }

    inline ActionTweenPtr operator->() const
    {
        return ptr;
    }

private:
    ActionTweenPtr ptr;
};

/// \~chinese
/// @brief 动画构造器
struct Tween
{
public:
    /// \~chinese
    /// @brief 构造相对位移动画
    /// @param dur 动画时长
    /// @param vector 移动向量
    static inline TweenHelper MoveBy(Duration dur, const Point& vector)
    {
        return TweenHelper(ActionMoveBy::Create(dur, vector));
    }

    /// \~chinese
    /// @brief 构造位移动画
    /// @param dur 动画时长
    /// @param pos 目的坐标
    static inline TweenHelper MoveTo(Duration dur, const Point& pos)
    {
        return TweenHelper(ActionMoveTo::Create(dur, pos));
    }

    /// \~chinese
    /// @brief 构造相对跳跃动画
    /// @param dur 动画时长
    /// @param vec 跳跃位移向量
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    static inline TweenHelper JumpBy(Duration dur, const Vec2& vec, float height, int jumps = 1)
    {
        return TweenHelper(ActionJumpBy::Create(dur, vec, height, jumps));
    }

    /// \~chinese
    /// @brief 构造跳跃动画
    /// @param dur 动画时长
    /// @param pos 目的坐标
    /// @param height 跳跃高度
    /// @param jumps 跳跃次数
    static inline TweenHelper JumpTo(Duration dur, const Point& pos, float height, int jumps = 1)
    {
        return TweenHelper(ActionJumpTo::Create(dur, pos, height, jumps));
    }

    /// \~chinese
    /// @brief 构造相对缩放动画
    /// @param dur 动画时长
    /// @param scale_x 横向缩放相对变化值
    /// @param scale_y 纵向缩放相对变化值
    static inline TweenHelper ScaleBy(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(ActionScaleBy::Create(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief 构造缩放动画
    /// @param dur 动画时长
    /// @param scale_x 横向缩放目标值
    /// @param scale_y 纵向缩放目标值
    static inline TweenHelper ScaleTo(Duration dur, float scale_x, float scale_y)
    {
        return TweenHelper(ActionScaleTo::Create(dur, scale_x, scale_y));
    }

    /// \~chinese
    /// @brief 构造透明度渐变动画
    /// @param dur 动画时长
    /// @param opacity 目标透明度
    static inline TweenHelper FadeTo(Duration dur, float opacity)
    {
        return TweenHelper(ActionFadeTo::Create(dur, opacity));
    }

    /// \~chinese
    /// @brief 构造淡入动画
    /// @param dur 动画时长
    static inline TweenHelper FadeIn(Duration dur)
    {
        return TweenHelper(ActionFadeIn::Create(dur));
    }

    /// \~chinese
    /// @brief 构造淡出动画
    /// @param dur 动画时长
    static inline TweenHelper FadeOut(Duration dur)
    {
        return TweenHelper(ActionFadeOut::Create(dur));
    }

    /// \~chinese
    /// @brief 构造相对旋转动画
    /// @param dur 动画时长
    /// @param rotation 角度相对变化值
    static inline TweenHelper RotateBy(Duration dur, float rotation)
    {
        return TweenHelper(ActionRotateBy::Create(dur, rotation));
    }

    /// \~chinese
    /// @brief 构造旋转动画
    /// @param dur 动画时长
    /// @param rotation 目标角度
    static inline TweenHelper RotateTo(Duration dur, float rotation)
    {
        return TweenHelper(ActionRotateTo::Create(dur, rotation));
    }

    /// \~chinese
    /// @brief 构造路径行走动画
    /// @param dur 持续时长
    /// @param path 路径形状
    /// @param rotating 是否沿路径切线方向旋转
    /// @param start 路径起点（百分比）
    /// @param end 路径终点（百分比）
    static inline TweenHelper Walk(Duration dur, ShapePtr path, bool rotating = false, float start = 0.f,
                                   float end = 1.f)
    {
        return TweenHelper(ActionWalk::Create(dur, path, rotating, start, end));
    }

    /// \~chinese
    /// @brief 构建帧动画
    /// @param dur 动画时长
    /// @param[in] frame_seq 序列帧
    static inline TweenHelper Animation(Duration dur, FrameSequencePtr frames)
    {
        return TweenHelper(Animation::Create(dur, frames));
    }

    /// \~chinese
    /// @brief 构造自定义动画
    /// @param dur 动画时长
    /// @param tween_func 动画回调函数
    static inline TweenHelper Custom(Duration dur, ActionCustom::TweenFunc tween_func)
    {
        return TweenHelper(ActionCustom::Create(dur, tween_func));
    }

    /// \~chinese
    /// @brief 构建延时动画
    /// @param delay 延时时长
    static inline ActionHelper Delay(Duration delay)
    {
        return ActionHelper(ActionDelay::Create(delay));
    }

    /// \~chinese
    /// @brief 动画组合
    /// @param actions 动画集合
    /// @param parallel 同步执行
    static inline ActionHelper Group(const Vector<ActionPtr>& actions, bool parallel = false)
    {
        return ActionHelper(ActionGroup::Create(actions, parallel));
    }
};

/** @} */
}  // namespace kiwano
