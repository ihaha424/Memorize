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

	D2D_POINT_2F parentPos = parent->GetComponentLocation();
	if (parentPos.x < screenSize.x / 2)
	{
		T.dx = screenSize.x / 2 - parentPos.x;
	}
	if (parentPos.x > mapSize.x - screenSize.x / 2)
	{
		T.dx = mapSize.x - screenSize.x / 2 - parentPos.x;
	}
	if (parentPos.y < screenSize.y / 2)
	{
		T.dy = screenSize.y / 2 - parentPos.y;
	}
	if (parentPos.y > mapSize.y - screenSize.y / 2)
	{
		T.dy = mapSize.y - screenSize.y / 2 - parentPos.y;
	}
}
