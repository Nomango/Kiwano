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
#include "Node.h"
#include "Image.h"

namespace easy2d
{
	// ����
	class Sprite
		: public Node
	{
		E2D_DISABLE_COPY(Sprite);

	public:
		Sprite();

		explicit Sprite(
			spImage const& image
		);

		explicit Sprite(
			Resource const& res
		);

		explicit Sprite(
			Resource const& res,
			const Rect& crop_rect	/* �ü����� */
		);

		explicit Sprite(
			String const& file_name
		);

		explicit Sprite(
			String const& file_name,
			const Rect& crop_rect	/* �ü����� */
		);

		virtual ~Sprite();

		// ����ͼƬ�ļ�
		bool Load(
			Resource const& res
		);

		// ����ͼƬ�ļ�
		bool Load(
			String const& file_name
		);

		// ����ͼƬ
		bool Load(
			spImage const& image
		);

		// ��ͼƬ�ü�Ϊ����
		void Crop(
			const Rect& crop_rect	/* �ü����� */
		);

		// ��ȡ Image ����
		spImage const& GetImage() const;

		// ��Ⱦ����
		virtual void OnDraw() override;

	protected:
		spImage image_;
	};
}