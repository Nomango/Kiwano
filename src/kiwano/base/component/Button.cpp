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

#include <kiwano/base/component/Button.h>
#include <kiwano/2d/Stage.h>
#include <kiwano/platform/Application.h>

namespace kiwano
{

ButtonBase::ButtonBase()
    : status_(Status::Normal)
{
    SetName("__KGE_BUTTON__");
}

ButtonBase::~ButtonBase()
{
}

void ButtonBase::SetStatus(Status status)
{
    if (status_ != status)
    {
        Status old_status = status_;

        if (status == Status::Normal)
        {
            OnEvent(Event::MouseOut);
            Application::GetInstance().GetWindow()->SetCursor(CursorType::Arrow);
        }
        else if (status == Status::Hover)
        {
            Application::GetInstance().GetWindow()->SetCursor(CursorType::Hand);

            if (old_status != Status::Pressed)
            {
                OnEvent(Event::MouseOver);
            }
        }
        else if (status == Status::Pressed)
        {
            OnEvent(Event::Pressed);
        }

        status_ = status;
    }
}

void ButtonBase::InitComponent(Actor* actor)
{
    Component::InitComponent(actor);
    if (actor)
    {
        actor->SetResponsible(true);
    }
}

void ButtonBase::DestroyComponent()
{
    Component::DestroyComponent();
}

void ButtonBase::HandleEvent(kiwano::Event* evt)
{
    if (evt->IsType<MouseHoverEvent>())
    {
        SetStatus(Status::Hover);
    }
    else if (evt->IsType<MouseOutEvent>())
    {
        SetStatus(Status::Normal);
    }
    else if (evt->IsType<MouseDownEvent>() && status_ == Status::Hover)
    {
        SetStatus(Status::Pressed);
    }
    else if (evt->IsType<MouseUpEvent>() && status_ == Status::Pressed)
    {
        SetStatus(Status::Hover);
    }
    else if (evt->IsType<MouseClickEvent>())
    {
        OnEvent(Event::Click);
    }
}

Button::Button() {}

Button::Button(const Callback& cb)
{
    SetCallback(cb);
}

void Button::OnEvent(Event evt)
{
    if (cb_)
    {
        cb_(this, evt);
    }
}

}  // namespace kiwano
