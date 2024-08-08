#include "GCameraComponent.h"

GCameraComponent::GCameraComponent()
{
	SetTickProperties(TICK_POST_UPDATE);
}

GCameraComponent::~GCameraComponent()
{
}

void GCameraComponent::PostUpdate(float _dt)
{
	CameraComponent::PostUpdate(_dt);

	Math::Vector2 parentPos = parent->GetComponentLocation();
	if (parentPos.x < screenSize.x / 2)
	{
		// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dx
		T._14 = screenSize.x / 2 - parentPos.x;
	}
	if (parentPos.x > mapSize.x - screenSize.x / 2)
	{
		// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dx
		T._14 = mapSize.x - screenSize.x / 2 - parentPos.x;
	}
	if (parentPos.y < screenSize.y / 2)
	{
		// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dy
		T._24 = screenSize.y / 2 - parentPos.y;
	}
	if (parentPos.y > mapSize.y - screenSize.y / 2)
	{
		// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dy
		T._24 = mapSize.y - screenSize.y / 2 - parentPos.y;
	}
}
