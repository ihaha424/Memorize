#include "ProgressBar.h"
#include "D2DRenderer.h"
#include "World.h"
#include "CameraComponent.h"
#include "ResourceManager.h"
#include "SpriteResource.h"

ProgressBar::ProgressBar(World* _world) : UIElement(_world)
{
	SetTickProperties(TICK_RENDER);
}

ProgressBar::~ProgressBar()
{
}

void ProgressBar::Render(D2DRenderer* _renderer)
{
	__super::Render(_renderer);

	if (fullImage == nullptr) 
		return;

	D2D1_RECT_F srcRect, dstRect;

	srcRect.left = 0;
	srcRect.right = fullImage->GetSize().width;
	srcRect.top = 0;
	srcRect.bottom = fullImage->GetSize().height;

	dstRect.left = -fullImage->GetSize().width / 2;
	dstRect.right = fullImage->GetSize().width / 2;
	dstRect.top = -fullImage->GetSize().height / 2;
	dstRect.bottom = fullImage->GetSize().height / 2;

	switch (dir)
	{
	case ProgressBarDir::BottomToTop:
		srcRect.top = fullImage->GetSize().height * (1 - value);
		dstRect.top = -fullImage->GetSize().height / 2 + fullImage->GetSize().height * (1 - value);
		break;
	case ProgressBarDir::LeftToRight:
		srcRect.right = fullImage->GetSize().width * value;
		dstRect.right = -fullImage->GetSize().width / 2 + fullImage->GetSize().width * value;
		break;
	case ProgressBarDir::RightToLeft:
		srcRect.left = fullImage->GetSize().width * (1 - value);
		dstRect.left = -fullImage->GetSize().width / 2 + fullImage->GetSize().width * (1 - value);
		break;
	}

	Math::Matrix transform = Math::Matrix::Identity;
	transform._41 = position.x;
	transform._42 = position.y;
	_renderer->PushTransform(transform);

	_renderer->GetRenderTarget()->DrawBitmap(
		fullImage,
		dstRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR, srcRect
	);
	_renderer->PopTransform();
	
}

void ProgressBar::SetFillImage(std::wstring _path)
{
	fullImage = ResourceManager::LoadResource<SpriteResource>(_path)->GetResource();
	if (!fullImage) {
		throw std::exception("BitmapScene: image asset is not found!");
	}
}