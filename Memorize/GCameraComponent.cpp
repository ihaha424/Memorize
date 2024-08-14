#include "GCameraComponent.h"

GCameraComponent::GCameraComponent(Actor* _owner) : CameraComponent(_owner)
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
	//if (parentPos.x < screenSize.x / 2)
	//{
	//	// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dx
	//	T._41 = screenSize.x / 2 - parentPos.x;
	//}
	//if (parentPos.x > mapSize.x - screenSize.x / 2)
	//{
	//	// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dx
	//	T._41 = mapSize.x - screenSize.x / 2 - parentPos.x;
	//}
	//if (parentPos.y < screenSize.y / 2)
	//{
	//	// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dy
	//	T._42 = screenSize.y / 2 - parentPos.y;
	//}
	//if (parentPos.y > mapSize.y - screenSize.y / 2)
	//{
	//	// Æ®·»½ºÆû ¸ÅÆ®¸¯½ºÀÇ dy
	//	T._42 = mapSize.y - screenSize.y / 2 - parentPos.y;
	//}
}
