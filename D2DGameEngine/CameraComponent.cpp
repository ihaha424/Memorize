#include "CameraComponent.h"
#include "Pawn.h"
#include "World.h"

const Math::Vector2 CameraComponent::screenSize = { 1920, 1080 };
const Math::Vector2 CameraComponent::mapSize = { 1920, 1080 };

CameraComponent::CameraComponent()
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(const float& deltaTime)
{
	Math::Vector2 center;
	center.x = std::clamp(center.x, screenSize.x / 2, mapSize.x - screenSize.x / 2);
	center.y = std::clamp(center.y, screenSize.y / 2, mapSize.y - screenSize.y / 2);

	T = R *
		D2D1::Matrix3x2F::Translation(center.x - screenSize.x / 2, center.y - screenSize.y / 2);
}

bool CameraComponent::InCameraArea(Actor* actor)
{
	//TODO 충돌 체크 
	return false;
}

