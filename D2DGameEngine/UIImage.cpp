#include "UIImage.h"
#include "D2DRenderer.h"
#include "ResourceManager.h"
#include "SpriteResource.h"

UIImage::UIImage(class World* _world) : UIElement(_world)
{
}

UIImage::~UIImage()
{
}

void UIImage::SetSprite(std::wstring _path)
{
	sprite = ResourceManager::LoadResource<SpriteResource>(_path)->GetResource();
	if (!sprite) {
		throw std::exception("BitmapScene: image asset is not found!");
	}

}

void UIImage::Render(D2DRenderer* _renderer)
{
	__super::Render(_renderer);

	Math::Matrix transform = Math::Matrix::Identity;
	transform._41 = position.x;
	transform._42 = position.y;
	_renderer->PushTransform(transform);

	D2D1_RECT_F DestRect = { 
		position.x - width / 2, 
		position.y - height / 2, 
		position.x + width / 2,
		position.y + height / 2 };

	_renderer->DrawSprite(sprite, DestRect, {0, 0, sprite->GetSize().width, sprite->GetSize().height});

	_renderer->PopTransform();
}
