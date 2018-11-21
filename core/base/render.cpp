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

#include "render.h"
#include "time.h"
#include "base.hpp"
#include "logs.h"
#include "Factory.h"
#include "Image.h"
#include "Transform.hpp"

namespace easy2d
{
	namespace devices
	{
		GraphicsDevice::GraphicsDevice()
			: fps_text_format_(nullptr)
			, fps_text_layout_(nullptr)
			, clear_color_(D2D1::ColorF(D2D1::ColorF::Black))
			, opacity_(1.f)
			, window_occluded_(false)
			, initialized_(false)
			, options_()
		{
		}

		GraphicsDevice::~GraphicsDevice()
		{
			E2D_LOG("Destroying graphics device");

			ClearImageCache();
		}

		void GraphicsDevice::Init(HWND hwnd, GraphicsOptions options, bool debug)
		{
			if (initialized_)
				return;

			E2D_LOG("Initing graphics device");

			options_ = options;

			CreateDeviceResources(hwnd);

			initialized_ = true;
		}

		void GraphicsDevice::BeginDraw(HWND hwnd)
		{
			CreateDeviceResources(hwnd);

			window_occluded_ = !!(render_target_->CheckWindowState() & D2D1_WINDOW_STATE_OCCLUDED);

			if (!window_occluded_)
			{
				render_target_->BeginDraw();
				render_target_->Clear(clear_color_);
			}
		}

		void GraphicsDevice::EndDraw()
		{
			if (!window_occluded_)
			{
				HRESULT hr = render_target_->EndDraw();

				if (hr == D2DERR_RECREATE_TARGET)
				{
					// 如果 Direct3D 设备在执行过程中消失，将丢弃当前的设备相关资源
					// 并在下一次调用时重建资源
					hr = S_OK;

					fps_text_format_ = nullptr;
					fps_text_layout_ = nullptr;
					text_renderer_ = nullptr;
					solid_brush_ = nullptr;
					render_target_ = nullptr;
				}

				ThrowIfFailed(hr);
			}
		}

		void GraphicsDevice::ClearImageCache()
		{
			bitmap_cache_.clear();
		}

		HRESULT GraphicsDevice::CreateLayer(cpLayer& layer)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			layer = nullptr;
			return render_target_->CreateLayer(&layer);
		}

		HRESULT GraphicsDevice::CreateSolidColorBrush(cpSolidColorBrush & brush) const
		{
			if (!render_target_)
				return E_UNEXPECTED;

			brush = nullptr;
			return render_target_->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::White),
				&brush
			);
		}

		HRESULT GraphicsDevice::DrawGeometry(
			cpGeometry const& geometry,
			Color const& stroke_color,
			float stroke_width,
			StrokeStyle stroke
		)
		{
			if (!solid_brush_ ||
				!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			solid_brush_->SetColor(stroke_color);
			auto stroke_style = Factory::Instance()->GetStrokeStyle(stroke);
			render_target_->DrawGeometry(
				geometry.Get(),
				solid_brush_.Get(),
				stroke_width,
				stroke_style.Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::FillGeometry(cpGeometry const & geometry, const Color & fill_color)
		{
			if (!solid_brush_ ||
				!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			solid_brush_->SetColor(fill_color);
			render_target_->FillGeometry(
				geometry.Get(),
				solid_brush_.Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawImage(spImage const & image)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			if (!image->GetBitmap())
				return S_OK;

			if (window_occluded_)
				return S_OK;

			render_target_->DrawBitmap(
				image->GetBitmap().Get(),
				D2D1::RectF(0.f, 0.f, image->GetWidth(), image->GetHeight()),
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				image->GetCropRect()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawBitmap(
			cpBitmap const& bitmap
		)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			// Do not crop bitmap 
			auto rect = D2D1::RectF(0.f, 0.f, bitmap->GetSize().width, bitmap->GetSize().height);
			render_target_->DrawBitmap(
				bitmap.Get(),
				rect,
				opacity_,
				D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
				rect
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::DrawTextLayout(cpTextLayout const& text_layout)
		{
			if (!text_renderer_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			return text_layout->Draw(nullptr, text_renderer_.Get(), 0, 0);
		}

		HRESULT GraphicsDevice::PushClip(const math::Matrix & clip_matrix, const Size & clip_size)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			render_target_->SetTransform(ConvertToD2DMatrix(clip_matrix));
			render_target_->PushAxisAlignedClip(
				D2D1::RectF(0, 0, clip_size.width, clip_size.height),
				D2D1_ANTIALIAS_MODE_PER_PRIMITIVE
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopClip()
		{
			if (!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			render_target_->PopAxisAlignedClip();
			return S_OK;
		}

		HRESULT GraphicsDevice::PushLayer(cpLayer const& layer, LayerProperties const& properties)
		{
			if (!render_target_ ||
				!solid_brush_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			render_target_->PushLayer(
				D2D1::LayerParameters(
					properties.area,
					nullptr,
					D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
					D2D1::Matrix3x2F::Identity(),
					properties.opacity,
					solid_brush_.Get(),
					D2D1_LAYER_OPTIONS_NONE
				),
				layer.Get()
			);
			return S_OK;
		}

		HRESULT GraphicsDevice::PopLayer()
		{
			if (!render_target_)
				return E_UNEXPECTED;

			if (window_occluded_)
				return S_OK;

			render_target_->PopLayer();
			return S_OK;
		}

		HRESULT GraphicsDevice::GetSize(Size & size)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			auto rtsize = render_target_->GetSize();
			size.width = rtsize.width;
			size.height = rtsize.height;
			return S_OK;
		}

		HRESULT GraphicsDevice::CreateBitmapFromFile(cpBitmap& bitmap, String const& file_path)
		{
			if (render_target_ == nullptr)
			{
				return E_UNEXPECTED;
			}

			size_t hash_code = std::hash<String>{}(file_path);
			if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
			{
				bitmap = bitmap_cache_[hash_code];
				return S_OK;
			}

			cpBitmap bitmap_tmp;
			HRESULT hr = Factory::Instance()->CreateBitmapFromFile(
				bitmap,
				render_target_,
				file_path
			);

			if (SUCCEEDED(hr))
			{
				bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
			}

			return hr;
		}

		HRESULT GraphicsDevice::CreateBitmapFromResource(cpBitmap& bitmap, Resource const& res)
		{
			if (render_target_ == nullptr)
			{
				return E_UNEXPECTED;
			}

			size_t hash_code = res.GetHashCode();
			if (bitmap_cache_.find(hash_code) != bitmap_cache_.end())
			{
				bitmap = bitmap_cache_[hash_code];
				return S_OK;
			}

			HRESULT hr = Factory::Instance()->CreateBitmapFromResource(
				bitmap,
				render_target_,
				res
			);

			if (SUCCEEDED(hr))
			{
				bitmap_cache_.insert(std::make_pair(hash_code, bitmap));
			}

			return hr;
		}

		HRESULT GraphicsDevice::CreateBitmapRenderTarget(cpBitmapRenderTarget & brt)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			brt = nullptr;
			return render_target_->CreateCompatibleRenderTarget(&brt);
		}

		HRESULT GraphicsDevice::Resize(UINT32 width, UINT32 height)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			render_target_->Resize(D2D1::SizeU(width, height));
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTransform(const math::Matrix & matrix)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			render_target_->SetTransform(ConvertToD2DMatrix(matrix));
			return S_OK;
		}

		HRESULT GraphicsDevice::SetOpacity(float opacity)
		{
			if (!render_target_)
				return E_UNEXPECTED;

			opacity_ = opacity;
			solid_brush_->SetOpacity(opacity);
			return S_OK;
		}

		HRESULT GraphicsDevice::SetTextStyle(
			Color const& color,
			bool has_outline,
			Color const& outline_color,
			float outline_width,
			StrokeStyle outline_stroke
		)
		{
			if (!text_renderer_)
				return E_UNEXPECTED;

			auto stroke_style = Factory::Instance()->GetStrokeStyle(outline_stroke);
			text_renderer_->SetTextStyle(
				color,
				has_outline,
				outline_color,
				outline_width,
				stroke_style.Get()
			);
			return S_OK;
		}

		void GraphicsDevice::SetBackgroundColor(const Color& color)
		{
			clear_color_ = color;
		}

		void GraphicsDevice::CreateDeviceResources(HWND hwnd)
		{
			if (!render_target_)
			{
				RECT rc;
				::GetClientRect(hwnd, &rc);

				D2D1_SIZE_U size = D2D1::SizeU(
					rc.right - rc.left,
					rc.bottom - rc.top
				);

				// 创建设备相关资源。这些资源应在 Direct2D 设备消失时重建
				// 创建一个 Direct2D 渲染目标
				ThrowIfFailed(
					Factory::Instance()->CreateHwndRenderTarget(
						render_target_,
						D2D1::RenderTargetProperties(),
						D2D1::HwndRenderTargetProperties(
							hwnd,
							size,
							options_.vsync ? D2D1_PRESENT_OPTIONS_NONE : D2D1_PRESENT_OPTIONS_IMMEDIATELY
						)
					)
				);

				render_target_->SetAntialiasMode(
					options_.antialias ? D2D1_ANTIALIAS_MODE_PER_PRIMITIVE : D2D1_ANTIALIAS_MODE_ALIASED
				);

				D2D1_TEXT_ANTIALIAS_MODE mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
				switch (options_.text_antialias)
				{
				case TextAntialias::Default:
					mode = D2D1_TEXT_ANTIALIAS_MODE_DEFAULT;
					break;
				case TextAntialias::ClearType:
					mode = D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE;
					break;
				case TextAntialias::GrayScale:
					mode = D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE;
					break;
				case TextAntialias::None:
					mode = D2D1_TEXT_ANTIALIAS_MODE_ALIASED;
					break;
				default:
					break;
				}
				render_target_->SetTextAntialiasMode(mode);
			}

			if (!solid_brush_)
			{
				ThrowIfFailed(
					render_target_->CreateSolidColorBrush(
						D2D1::ColorF(D2D1::ColorF::White),
						&solid_brush_
					)
				);
			}

			if (!text_renderer_)
			{
				ThrowIfFailed(
					Factory::Instance()->CreateTextRenderer(
						text_renderer_,
						render_target_,
						solid_brush_
					)
				);
			}
		}

	}
}
