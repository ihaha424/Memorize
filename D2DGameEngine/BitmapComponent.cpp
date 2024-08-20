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

	float halfWidth = GetFrameWidth() / 2.f;
	float halfHeight = GetFrameHeight() / 2.f;

	D2D_RectF dest{
		.left = -halfWidth,
		.top = -halfHeight,
		.right = halfWidth,
		.bottom = halfHeight
	};

	_renderer->DrawSprite(
		sprite->GetResource(), 
		dest, 
		frame,
		opacity
	);

#ifndef NDEBUG
	_renderer->DrawBorder(
		{dest.left, dest.top},
		{dest.right, dest.bottom},
		D2D_Color::Green
	);
#endif

	_renderer->PopTransform();

#ifndef NDEBUG // debug bounds
	_renderer->DrawBorder(
		{ bounds.center.x - (bounds.boxExtent.width / 2.f), bounds.center.y - (bounds.boxExtent.height / 2.f) },
		{ bounds.center.x + (bounds.boxExtent.width / 2.f), bounds.center.y + (bounds.boxExtent.height / 2.f) },
		D2D_Color::Blue
	);
#endif
}
