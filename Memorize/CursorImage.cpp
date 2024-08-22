#include "CursorImage.h"
#include "D2DGameEngine/D2DRenderer.h"

CursorImage::CursorImage(World* _world) : UIImage(_world)
{
	SetSprite(L"TestResource/Cursors/Cursor_Default.png");

}

CursorImage::~CursorImage()
{
}

void CursorImage::Render(D2DRenderer* _renderer)
{
	Math::Matrix transform = Math::Matrix::Identity;
	transform._41 = position.x;
	transform._42 = position.y;

	_renderer->PushTransform(transform);

	D2D1_RECT_F DestRect = {
		0,
		0,
		width,
		height};

	_renderer->DrawSprite(sprite, DestRect, { 0, 0, sprite->GetSize().width, sprite->GetSize().height });

	_renderer->PopTransform();
}
