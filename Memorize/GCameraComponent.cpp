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
	prevBossPos = bossPos;
	initialDistance = (parentPos - bossPos).Length();

}
#include "../D2DGameEngine/Mouse.h"
void GCameraComponent::PostUpdate(float _dt)
{
	CameraComponent::PostUpdate(_dt);

	Math::Vector2 parentPos = parent->GetComponentLocation();


	if (isMove)
	{
		moveSecond -= _dt;
		Math::Vector2 destinationCameraPos = ((prevBossPos - prevPlayerPos) * 0.5f - GetComponentLocation());
		destinationCameraPos.Normalize();
		Translate(destinationCameraPos * moveSpeed * _dt);
		if (moveSecond < 0.f)
			isMove = false;
		return;
	}

	Math::Vector2 bossPos = boss->rootComponent->GetComponentLocation();

	if ((prevBossPos - bossPos).Length() > 100.f)
	{
		isMove = true;
		moveSecond = 1.f;
		moveSpeed = (prevBossPos - bossPos).Length() / (1.f + cameraScale);	// (prevBossPos - bossPos).Length() / moveSecond;
		prevBossPos = bossPos;
		prevPlayerPos = parentPos;
		return;
	}

	prevBossPos = bossPos;
	Math::Vector2 destinationCameraPos = (bossPos - parentPos) * 0.5f;

	//Scale
	cameraScale = (parentPos - bossPos).Length() / initialDistance;
	cameraScale -= 1;
	cameraScale *= 0.3f;
	if (cameraScale < 0.5f)
		cameraScale = 0.5f;
	SetScale(1.f + cameraScale, 1.f + cameraScale);


	//Position
	SetTranslation(0, 0);
	Translate(destinationCameraPos);

}
