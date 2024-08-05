#pragma once

#include <d2d1helper.h>
#include <d2d1_1helper.h>

using D2D_Color = D2D1::ColorF;
using D2D_TColor = D2D1_COLOR_F;

using D2D_RectF = D2D1_RECT_F;
using D2D_RectU = D2D1_RECT_U;

using D2D_Mat3x2F = D2D1::Matrix3x2F;
using D2D_TMat3x2F = D2D1_MATRIX_3X2_F;
using D2D_Point2F = D2D1_POINT_2F;
using D2D_Vec2F = D2D1_VECTOR_2F;
using D2D_Sprite = ID2D1Bitmap;

struct Extent2D {
	float width{ 0.f };
	float height{ 0.f };
};

using uint = unsigned int;

