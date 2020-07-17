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
#include <kiwano/2d/action/Action.h>
#include <kiwano/utils/Logger.h>

namespace kiwano
{
/// \~chinese
/// @brief 缓动函数
using EaseFunc = Function<float(float)>;

/// \~chinese
/// @brief 缓动函数枚举
/// @details 查看 https://easings.net 获取更多信息
struct Ease
{
    static KGE_API EaseFunc Linear;        ///< 线性
    static KGE_API EaseFunc EaseIn;        ///< 由慢变快
    static KGE_API EaseFunc EaseOut;       ///< 由快变慢
    static KGE_API EaseFunc EaseInOut;     ///< 由慢变快, 再由快变慢
    static KGE_API EaseFunc ExpoIn;        ///< 由慢变极快
    static KGE_API EaseFunc ExpoOut;       ///< 由极快变慢
    static KGE_API EaseFunc ExpoInOut;     ///< 由慢至极快, 再由极快边慢
    static KGE_API EaseFunc ElasticIn;     ///< 自起点赋予弹性
    static KGE_API EaseFunc ElasticOut;    ///< 自终点赋予弹性
    static KGE_API EaseFunc ElasticInOut;  ///< 再起点和终点赋予弹性
    static KGE_API EaseFunc BounceIn;      ///< 自起点赋予反弹力
    static KGE_API EaseFunc BounceOut;     ///< 自终点赋予反弹力
    static KGE_API EaseFunc BounceInOut;   ///< 在起点和终点赋予反弹力
    static KGE_API EaseFunc BackIn;
    static KGE_API EaseFunc BackOut;
    static KGE_API EaseFunc BackInOut;
    static KGE_API EaseFunc QuadIn;
    static KGE_API EaseFunc QuadOut;
    static KGE_API EaseFunc QuadInOut;
    static KGE_API EaseFunc CubicIn;
    static KGE_API EaseFunc CubicOut;
    static KGE_API EaseFunc CubicInOut;
    static KGE_API EaseFunc QuartIn;
    static KGE_API EaseFunc QuartOut;
    static KGE_API EaseFunc QuartInOut;
    static KGE_API EaseFunc QuintIn;
    static KGE_API EaseFunc QuintOut;
    static KGE_API EaseFunc QuintInOut;
    static KGE_API EaseFunc SineIn;
    static KGE_API EaseFunc SineOut;
    static KGE_API EaseFunc SineInOut;
};

KGE_DECLARE_SMART_PTR(ActionTween);
KGE_DECLARE_SMART_PTR(ActionMoveBy);
KGE_DECLARE_SMART_PTR(ActionMoveTo);
KGE_DECLARE_SMART_PTR(ActionJumpBy);
KGE_DECLARE_SMART_PTR(ActionJumpTo);
KGE_DECLARE_SMART_PTR(ActionScaleBy);
KGE_DECLARE_SMART_PTR(ActionScaleTo);
KGE_DECLARE_SMART_PTR(ActionFadeTo);
KGE_DECLARE_SMART_PTR(ActionFadeIn);
KGE_DECLARE_SMART_PTR(ActionFadeOut);
KGE_DECLARE_SMART_PTR(ActionRotateBy);
KGE_DECLARE_SMART_PTR(ActionRotateTo);
KGE_DECLARE_SMART_PTR(ActionCustom);

/**
 * \addtogroup Actions
 * @{
 */

/// \~chinese
/// @brief 补间动画
class KGE_API ActionTween : public Action
{
public:
    ActionTween();

    /// \~chinese
    /// @brief 补间动画
    /// @param duration 动画时长
    /// @param func 动画速度缓动函数
    ActionTween(Duration duration, EaseFunc ease);

    /// \~chinese
    /// @brief 获取动画时长
    Duration GetDuration() const;

    /// \~chinese
    /// @brief 设置动画时长
    void SetDuration(Duration duration);

    /// \~chinese
    /// @brief 获取动画速度缓动函数
    const EaseFunc& GetEaseFunc() const;

    /// \~chinese
    /// @brief 设置动画速度缓动函数
    void SetEaseFunc(const EaseFunc& func);

protected:
    void Update(Actor* target, Duration dt) override;

    virtual void UpdateTween(Actor* target, float percent) = 0;

    ActionPtr DoClone(ActionTweenPtr to) const;

private:
    Duration dur_;
    EaseFunc ease_func_;
};

/// \~chinese
/// @brief 相对位移动画
class KGE_API ActionMoveBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建相对位移动画
    /// @param duration 动画时长
    /// @param displacement 位移向量
    static ActionMoveByPtr Create(Duration duration, const Vec2& displacement);

    ActionMoveBy();

    /// \~chinese
    /// @brief 获取位移向量
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief 设置位移向量
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    Point start_pos_;
    Point prev_pos_;
    Vec2  displacement_;
};

/// \~chinese
/// @brief 位移动画
class KGE_API ActionMoveTo : public ActionMoveBy
{
public:
    /// \~chinese
    /// @brief 创建位移动画
    /// @param duration 动画时长
    /// @param distination 目的坐标
    static ActionMoveToPtr Create(Duration duration, const Point& distination);

    ActionMoveTo();

    /// \~chinese
    /// @brief 获取目的坐标
    Point GetDistination() const;

    /// \~chinese
    /// @brief 设置目的坐标
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionMoveTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};

/// \~chinese
/// @brief 相对跳跃动画
class KGE_API ActionJumpBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建相对跳跃动画
    /// @param duration 动画时长
    /// @param displacement 跳跃位移向量
    /// @param height 跳跃高度
    /// @param count 跳跃次数
    static ActionJumpByPtr Create(Duration duration, const Vec2& displacement, float height, int count = 1,
                                  EaseFunc ease = nullptr);

    ActionJumpBy();

    /// \~chinese
    /// @brief 获取跳跃位移
    Vec2 GetDisplacement() const;

    /// \~chinese
    /// @brief 获取跳跃高度
    float GetJumpHeight() const;

    /// \~chinese
    /// @brief 获取跳跃次数
    int GetJumpCount() const;

    /// \~chinese
    /// @brief 设置跳跃位移
    void SetDisplacement(const Vec2& displacement);

    /// \~chinese
    /// @brief 设置跳跃高度
    void SetJumpHeight(float height);

    /// \~chinese
    /// @brief 设置跳跃次数
    void SetJumpCount(int count);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float height_;
    int   jump_count_;
    Point start_pos_;
    Point displacement_;
    Point prev_pos_;
};

/// \~chinese
/// @brief 跳跃动画
class KGE_API ActionJumpTo : public ActionJumpBy
{
public:
    /// \~chinese
    /// @brief 创建跳跃动画
    /// @param duration 动画时长
    /// @param distination 目的坐标
    /// @param height 跳跃高度
    /// @param count 跳跃次数
    static ActionJumpToPtr Create(Duration duration, const Point& distination, float height, int count = 1,
                                  EaseFunc ease = nullptr);

    ActionJumpTo();

    /// \~chinese
    /// @brief 获取目的坐标
    Point GetDistination() const;

    /// \~chinese
    /// @brief 设置目的坐标
    void SetDistination(const Point& distination);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionJumpTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    Point distination_;
};

/// \~chinese
/// @brief 相对缩放动画
class KGE_API ActionScaleBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建相对缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放相对变化值
    /// @param scale_y 纵向缩放相对变化值
    static ActionScaleByPtr Create(Duration duration, float scale_x, float scale_y);

    ActionScaleBy();

    /// \~chinese
    /// @brief 获取横向缩放相对变化值
    float GetScaleX() const;

    /// \~chinese
    /// @brief 获取横向缩放相对变化值
    float GetScaleY() const;

    /// \~chinese
    /// @brief 设置纵向缩放相对变化值
    void SetScaleX(float scale_x);

    /// \~chinese
    /// @brief 设置纵向缩放相对变化值
    void SetScaleY(float scale_y);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_scale_x_;
    float start_scale_y_;
    float delta_x_;
    float delta_y_;
};

/// \~chinese
/// @brief 缩放动画
class KGE_API ActionScaleTo : public ActionScaleBy
{
public:
    /// \~chinese
    /// @brief 创建缩放动画
    /// @param duration 动画时长
    /// @param scale_x 横向缩放目标值
    /// @param scale_y 纵向缩放目标值
    static ActionScaleToPtr Create(Duration duration, float scale_x, float scale_y);

    ActionScaleTo();

    /// \~chinese
    /// @brief 获取横向缩放目标值
    float GetTargetScaleX() const;

    /// \~chinese
    /// @brief 获取横向缩放目标值
    float GetTargetScaleY() const;

    /// \~chinese
    /// @brief 设置纵向缩放目标值
    void SetTargetScaleX(float scale_x);

    /// \~chinese
    /// @brief 设置纵向缩放目标值
    void SetTargetScaleY(float scale_y);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionScaleTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_scale_x_;
    float end_scale_y_;
};

/// \~chinese
/// @brief 透明度渐变动画
class KGE_API ActionFadeTo : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建透明度渐变动画
    /// @param duration 动画时长
    /// @param opacity 目标透明度
    static ActionFadeToPtr Create(Duration duration, float opacity);

    ActionFadeTo();

    /// \~chinese
    /// @brief 获取目标透明度
    float GetTargetOpacity() const;

    /// \~chinese
    /// @brief 设置目标透明度
    void SetTargetOpacity(float opacity);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionFadeTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    float start_val_;
    float delta_val_;
    float end_val_;
};

/// \~chinese
/// @brief 淡入动画
class KGE_API ActionFadeIn : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief 创建淡入动画
    /// @param duration 动画时长
    static ActionFadeInPtr Create(Duration duration);
};

/// \~chinese
/// @brief 淡出动画
class KGE_API ActionFadeOut : public ActionFadeTo
{
public:
    /// \~chinese
    /// @brief 创建淡出动画
    /// @param duration 动画时长
    static ActionFadeOutPtr Create(Duration duration);
};

/// \~chinese
/// @brief 相对旋转动画
class KGE_API ActionRotateBy : public ActionTween
{
public:
    /// \~chinese
    /// @brief 创建相对旋转动画
    /// @param duration 动画时长
    /// @param rotation 角度相对变化值
    static ActionRotateByPtr Create(Duration duration, float rotation);

    ActionRotateBy();

    /// \~chinese
    /// @brief 获取角度相对变化值
    float GetRotation() const;

    /// \~chinese
    /// @brief 设置角度相对变化值
    void SetRotation(float rotation);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override;

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

protected:
    float start_val_;
    float delta_val_;
};

/// \~chinese
/// @brief 旋转动画
class KGE_API ActionRotateTo : public ActionRotateBy
{
public:
    /// \~chinese
    /// @brief 创建旋转动画
    /// @param duration 动画时长
    /// @param rotation 目标角度
    static ActionRotateToPtr Create(Duration duration, float rotation);

    ActionRotateTo();

    /// \~chinese
    /// @brief 获取目标角度
    float GetTargetRotation() const;

    /// \~chinese
    /// @brief 设置目标角度
    void SetTargetRotation(float rotation);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    virtual ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionRotateTo");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

private:
    float end_val_;
};

/// \~chinese
/// @brief 自定义动画
class KGE_API ActionCustom : public ActionTween
{
public:
    /// \~chinese
    /// @brief 动画回调函数
    /// @details 在动画更新时回调该函数，第一个参数是执行动画的目标，第二个参数是动画进度（0.0 - 1.0）
    using TweenFunc = Function<void(Actor* /* target */, float /* percent */)>;

    /// \~chinese
    /// @brief 创建自定义动画
    /// @param duration 动画时长
    /// @param tween_func 动画回调函数
    static ActionCustomPtr Create(Duration duration, TweenFunc tween_func);

    ActionCustom();

    /// \~chinese
    /// @brief 获取动画回调函数
    TweenFunc GetTweenFunc() const;

    /// \~chinese
    /// @brief 设置动画回调函数
    void SetTweenFunc(const TweenFunc& tween_func);

    /// \~chinese
    /// @brief 获取该动画的拷贝对象
    ActionPtr Clone() const override;

    /// \~chinese
    /// @brief 获取该动画的倒转
    ActionPtr Reverse() const override
    {
        KGE_ERRORF("Reverse() not supported in ActionCustom");
        return nullptr;
    }

protected:
    void Init(Actor* target) override;

    void UpdateTween(Actor* target, float percent) override;

private:
    TweenFunc tween_func_;
};

/** @} */

inline const EaseFunc& ActionTween::GetEaseFunc() const
{
    return ease_func_;
}

inline Duration ActionTween::GetDuration() const
{
    return dur_;
}

inline void ActionTween::SetDuration(Duration duration)
{
    dur_ = duration;
}

inline void ActionTween::SetEaseFunc(const EaseFunc& func)
{
    ease_func_ = func;
}

inline Vec2 ActionMoveBy::GetDisplacement() const
{
    return displacement_;
}

inline void ActionMoveBy::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline Point ActionMoveTo::GetDistination() const
{
    return distination_;
}

inline void ActionMoveTo::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline Vec2 ActionJumpBy::GetDisplacement() const
{
    return displacement_;
}

inline float ActionJumpBy::GetJumpHeight() const
{
    return height_;
}

inline int ActionJumpBy::GetJumpCount() const
{
    return jump_count_;
}

inline void ActionJumpBy::SetDisplacement(const Vec2& displacement)
{
    displacement_ = displacement;
}

inline void ActionJumpBy::SetJumpHeight(float height)
{
    height_ = height;
}

inline void ActionJumpBy::SetJumpCount(int count)
{
    jump_count_ = count;
}

inline Point ActionJumpTo::GetDistination() const
{
    return distination_;
}

inline void ActionJumpTo::SetDistination(const Point& distination)
{
    distination_ = distination;
}

inline float ActionScaleBy::GetScaleX() const
{
    return delta_x_;
}

inline float ActionScaleBy::GetScaleY() const
{
    return delta_y_;
}

inline void ActionScaleBy::SetScaleX(float scale_x)
{
    delta_x_ = scale_x;
}

inline void ActionScaleBy::SetScaleY(float scale_y)
{
    delta_y_ = scale_y;
}

inline float ActionScaleTo::GetTargetScaleX() const
{
    return end_scale_x_;
}

inline float ActionScaleTo::GetTargetScaleY() const
{
    return end_scale_y_;
}

inline void ActionScaleTo::SetTargetScaleX(float scale_x)
{
    end_scale_x_ = scale_x;
}

inline void ActionScaleTo::SetTargetScaleY(float scale_y)
{
    end_scale_y_ = scale_y;
}

inline float ActionFadeTo::GetTargetOpacity() const
{
    return end_val_;
}

inline void ActionFadeTo::SetTargetOpacity(float opacity)
{
    end_val_ = opacity;
}

inline float ActionRotateBy::GetRotation() const
{
    return delta_val_;
}

inline void ActionRotateBy::SetRotation(float rotation)
{
    delta_val_ = rotation;
}

inline float ActionRotateTo::GetTargetRotation() const
{
    return end_val_;
}

inline void ActionRotateTo::SetTargetRotation(float rotation)
{
    end_val_ = rotation;
}

inline ActionCustom::TweenFunc ActionCustom::GetTweenFunc() const
{
    return tween_func_;
}

inline void ActionCustom::SetTweenFunc(const TweenFunc& tween_func)
{
    tween_func_ = tween_func;
}

}  // namespace kiwano
