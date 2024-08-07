#include "BitmapComponent.h"
#include "World.h"
#include "CameraComponent.h"

#include "D2DRenderer.h"

Math::Matrix BitmapComponent::GetWorldTransform() const {
	return imageTransform * Super::GetWorldTransform();
}

void BitmapComponent::Render(D2DRenderer* _renderer)
{
	_renderer->PushTransform(GetWorldTransform());

	D2D_RectF dest{
		.left = -GetFrameWidth() / 2.f,
		.top = -GetFrameHeight() / 2.f,
		.right = GetFrameWidth() / 2.f,
		.bottom = GetFrameHeight() / 2.f
	};

	_renderer->DrawSprite(sprite->GetResource(), dest, frame);

#ifndef NDEBUG
	_renderer->DrawBorder(
		{dest.left, dest.top},
		{dest.right, dest.bottom},
		D2D_Color::Green
	);
#endif

	_renderer->PopTransform();
}
