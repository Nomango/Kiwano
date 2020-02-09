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
#include <kiwano/render/Renderer.h>
#include <kiwano/render/DirectX/FontCollectionLoader.h>
#include <kiwano/render/DirectX/RenderContextImpl.h>

#if defined(KGE_USE_DIRECTX10)
#include <kiwano/render/DirectX/D3D10DeviceResources.h>
#else
#include <kiwano/render/DirectX/D3D11DeviceResources.h>
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
typedef ID3D10DeviceResources ID3DDeviceResources;
#else
typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

class KGE_API RendererImpl
    : public Renderer
{
public:
    static RendererImpl& GetInstance();

    void CreateTexture(Texture& texture, String const& file_path);

    void CreateTexture(Texture& texture, Resource const& resource);

    void CreateGifImage(GifImage& gif, String const& file_path);

    void CreateGifImage(GifImage& gif, Resource const& resource);

    void CreateGifImageFrame(GifImage::Frame& frame, GifImage const& gif, size_t frame_index);

    void CreateFontCollection(Font& font, String const& file_path);

    void CreateFontCollection(Font& font, Resource const& res);

    void CreateTextFormat(TextLayout& layout);

    void CreateTextLayout(TextLayout& layout);

    void CreateLineShape(Shape& shape, Point const& begin_pos, Point const& end_pos);

    void CreateRectShape(Shape& shape, Rect const& rect);

    void CreateRoundedRectShape(Shape& shape, Rect const& rect, Vec2 const& radius);

    void CreateEllipseShape(Shape& shape, Point const& center, Vec2 const& radius);

    void CreateShapeSink(ShapeSink& sink);

    void CreateBrush(Brush& brush, Color const& color);

    void CreateBrush(Brush& brush, LinearGradientStyle const& style);

    void CreateBrush(Brush& brush, RadialGradientStyle const& style);

    void CreateStrokeStyle(StrokeStyle& stroke_style, CapStyle cap, LineJoinStyle line_join, const float* dash_array,
                           size_t dash_size, float dash_offset);

    TextureRenderContextPtr CreateTextureRenderContext(const Size* desired_size = nullptr);

public:
    void BeginDraw();

    void EndDraw();

    void Clear();

    void Present();

    RenderContext& GetContext();

    /// \~chinese
    /// @brief ��ȡDirect2D�豸��Դ
    ID2DDeviceResources* GetD2DDeviceResources();

    /// \~chinese
    /// @brief ��ȡDirect3D�豸��Դ
    ID3DDeviceResources* GetD3DDeviceResources();

public:
    void SetupComponent() override;

    void DestroyComponent() override;

    void HandleEvent(Event* evt) override;

private:
    RendererImpl();

    HRESULT HandleDeviceLost();

    void Resize(uint32_t width, uint32_t height);

private:
    RenderContextImplPtr                  render_ctx_;
    ComPtr<ID2DDeviceResources>           d2d_res_;
    ComPtr<ID3DDeviceResources>           d3d_res_;
    ComPtr<IFontCollectionLoader>         font_collection_loader_;
    ComPtr<IResourceFontFileLoader>       res_font_file_loader_;
    ComPtr<IResourceFontCollectionLoader> res_font_collection_loader_;
};

/** @} */

inline RenderContext& RendererImpl::GetContext()
{
    return *render_ctx_;
}

inline ID2DDeviceResources* RendererImpl::GetD2DDeviceResources()
{
    KGE_ASSERT(d2d_res_);
    return d2d_res_.get();
}

inline ID3DDeviceResources* RendererImpl::GetD3DDeviceResources()
{
    KGE_ASSERT(d3d_res_);
    return d3d_res_.get();
}

}  // namespace kiwano