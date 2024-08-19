#include "GCameraComponent.h"
#include "Boss.h"
#include "../D2DGameEngine/World.h"


GCameraComponent::GCameraComponent(Actor* _owner) : CameraComponent(_owner)
{
	SetTickProperties(TICK_POST_UPDATE);
}

GCameraComponent::~GCameraComponent()
{
}

void GCameraComponent::BeginPlay()
{
	__super::BeginPlay();
	boss = GetOwner()->GetWorld()->FindActorByType<Boss>();
	Math::Vector2 parentPos = parent->GetComponentLocation();
	Math::Vector2 bossPos = boss->rootComponent->GetComponentLocation();
	initialDistance = (parentPos - bossPos).Length();

}
#include "../D2DGameEngine/Mouse.h"
void GCameraComponent::PostUpdate(float _dt)
{
	CameraComponent::PostUpdate(_dt);

	Math::Vector2 parentPos = parent->GetComponentLocation();
	Math::Vector2 bossPos = boss->rootComponent->GetComponentLocation();
	Math::Vector2 cameraPos = (bossPos - parentPos) * 0.5f;
	float cameraScale = (parentPos - bossPos).Length() / initialDistance;
	cameraScale -= 1;
	cameraScale *= 0.3f;
	if (cameraScale < 0.5f)
		cameraScale = 0.5f;
	SetScale(1.f + cameraScale, 1.f + cameraScale);
	SetTranslation(0, 0);
	Translate(cameraPos.x, cameraPos.y);


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
