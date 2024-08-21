#include "CameraComponent.h"
#include "Pawn.h"
#include "World.h"

const Math::Vector2 CameraComponent::screenSize = { 1920, 1080 };
const Math::Vector2 CameraComponent::mapSize = { 2880, 1920 };

CameraComponent::CameraComponent(Actor* _owner) : PrimitiveComponent(_owner)
{
}

CameraComponent::~CameraComponent()
{
	if(GetWorld()->GetMainCamera() == this)
		GetWorld()->SetMainCamera(nullptr);
}

void CameraComponent::Update(float _dt)
{

}

bool CameraComponent::InCameraArea(Actor* actor)
{
	//TODO �浹 üũ 
	return true;
}

