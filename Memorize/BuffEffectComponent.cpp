#include "BuffEffectComponent.h"
#include "Player.h"
#include "GPlayerController.h"

BuffEffectComponent::BuffEffectComponent(Actor* _owner) : AnimationBitmapComponent(_owner)
{
}

BuffEffectComponent::~BuffEffectComponent()
{
}

void BuffEffectComponent::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;
	if (elapsedTime > duration)
	{
		player = static_cast<Player*>(GetOwner());
		GPlayerController* controller = static_cast<GPlayerController*>(player->GetController());
		controller->EndSkill();
		elapsedTime = 0.f;
		SetStatus(OS_INACTIVE);
		duration = 0.5f;
	}
}
