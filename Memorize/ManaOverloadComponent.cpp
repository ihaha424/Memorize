#include "ManaOverloadComponent.h"
#include "Player.h"
#include "GPlayerController.h"

ManaOverloadComponent::ManaOverloadComponent(Actor* _owner) : AnimationBitmapComponent(_owner)
{
	SetTranslation(0, -200);
	SetScale(0.5f, 0.5f);
	SetSprite(L"TestResource/Player/Skill/Skill_ManaOverload.png");
	SliceSpriteSheet(400, 350, 0, 0, 0, 0);
	SetFrameDurations({ 10.f / (22 * 12) });
	Trigger(true);
	SetLoop(true);
}

ManaOverloadComponent::~ManaOverloadComponent()
{
}

void ManaOverloadComponent::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	//std::cout << elapsedTime << std::endl;
	if (elapsedTime > duration)
	{
		elapsedTime = 0.f;

		GPlayerController* controller = static_cast<GPlayerController*>(static_cast<Player*>(GetOwner())->GetController());
		controller->bManaOverload = false;

		SetStatus(OS_INACTIVE);
		Trigger(false);
		Trigger(true);
	}
}
