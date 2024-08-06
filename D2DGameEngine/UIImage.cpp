#include "UIImage.h"
#include "D2DRenderer.h"

UIImage::UIImage(class World* _world) : UIElement(_world)
{
}

UIImage::~UIImage()
{
}

void UIImage::Render(D2DRenderer* _renderer)
{
	__super::Render(_renderer);

	D2D_Mat3x2F transform = D2D_Mat3x2F::Identity();
	transform._31 = position.x;
	transform._32 = position.y;
	_renderer->PushTransform(transform);

	D2D1_RECT_F DestRect = { 
		position.x - width/2, 
		position.y - height/2, 
		position.x + width / 2,
		position.y + height / 2 };

	_renderer->DrawSprite(sprite, DestRect, D2D_RectF(0));

	_renderer->PopTransform();
}
