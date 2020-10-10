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

#include <kiwano/2d/animation/KeyFrame.h>

namespace kiwano
{

KeyFrame::KeyFrame() {}

KeyFrame::KeyFrame(const String& file_path)
{
    Load(file_path);
}

KeyFrame::KeyFrame(const Resource& res)
{
    Load(res);
}

KeyFrame::KeyFrame(TexturePtr texture)
{
    SetTexture(texture);
}

KeyFrame::KeyFrame(const String& file_path, const Rect& crop_rect)
    : KeyFrame(file_path)
{
    SetCropRect(crop_rect);
}

KeyFrame::KeyFrame(const Resource& res, const Rect& crop_rect)
    : KeyFrame(res)
{
    SetCropRect(crop_rect);
}

KeyFrame::KeyFrame(TexturePtr texture, const Rect& crop_rect)
    : KeyFrame(texture)
{
    SetCropRect(crop_rect);
}

bool KeyFrame::Load(const String& file_path)
{
    TexturePtr texture = Texture::Preload(file_path);
    if (texture->IsValid())
    {
        SetTexture(texture);
        return true;
    }
    return false;
}

bool KeyFrame::Load(const Resource& res)
{
    TexturePtr texture = Texture::Preload(res);
    if (texture->IsValid())
    {
        SetTexture(texture);
        return true;
    }
    return false;
}

void KeyFrame::SetCropRect(const Rect& crop_rect)
{
    if (texture_->IsValid())
    {
        auto bitmap_size          = texture_->GetSize();
        crop_rect_.left_top.x     = std::min(std::max(crop_rect.left_top.x, 0.f), bitmap_size.x);
        crop_rect_.left_top.y     = std::min(std::max(crop_rect.left_top.y, 0.f), bitmap_size.y);
        crop_rect_.right_bottom.x = std::min(std::max(crop_rect.right_bottom.x, 0.f), bitmap_size.x);
        crop_rect_.right_bottom.y = std::min(std::max(crop_rect.right_bottom.y, 0.f), bitmap_size.y);
    }
}

void KeyFrame::SetTexture(TexturePtr texture)
{
    texture_ = texture;
    if (texture_->IsValid())
    {
        crop_rect_.left_top.x = crop_rect_.left_top.y = 0;
        crop_rect_.right_bottom.x                     = texture_->GetWidth();
        crop_rect_.right_bottom.y                     = texture_->GetHeight();
    }
}
}  // namespace kiwano