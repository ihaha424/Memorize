#include "ElementalMasterComponent.h"
#include "Player.h"
#include "GPlayerController.h"
#include "PlayerFSMComponent.h"
#include "ProjectileSkill.h"
#include "BuffEffectComponent.h"
#include "D2DGameEngine/AnimationEffect.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/World.h"

ElementalMasterComponent::ElementalMasterComponent(Actor* _owner) : IComponent(_owner)
{
	SetTickProperties(TICK_UPDATE);
}

ElementalMasterComponent::~ElementalMasterComponent()
{
}

void ElementalMasterComponent::BeginPlay()
{
	__super::BeginPlay();

	
}

void ElementalMasterComponent::Update(float _dt)
{

	controller = static_cast<GPlayerController*>(GetOwner());
	

	if (controller->bElementalMaster == false)
	{
		bSkillReady = false;
		elapsedTime = 0.f;
		lockedEffect->SetAliveTime(0.f);
		bStartedLockAnim = false;

		SetStatus(OS_INACTIVE);
		return;
	}


	elapsedTime += _dt;
	if (elapsedTime > 0.5f)
	{
		lockedEffect->GetAnimationBitmapComponent()->SetOpacity(1.f);

	}


	if (bSkillReady && controller->bNowAttacking)
	{
		Skill* nowSkill = controller->FindCurSkiil();
		if (nowSkill == nullptr || static_cast<ProjectileSkill*>(nowSkill)->bEnd == true)
		{
			bSkillReady = false;
			controller->bNowAttacking = false;
			controller->InitializeSkillInfo();
		}
		return;
	}


	if (!bSkillReady )
	{
		controller->playerFSMComponent->SetNextState(L"PlayerCasting");
		controller->SetRandomSkillReady();
		if (controller->playerFSMComponent->GetCurState() == L"PlayerCasting")
		{
			PlayerCasting* state = static_cast<PlayerCasting*>(controller->playerFSMComponent->GetCurStateClass());
			state->index = state->commandList.size();
			bSkillReady = true;
		}

	}

}

void ElementalMasterComponent::Initialize(Player* player)
{
	{
		lockedEffect = GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		lockedEffect->SetSprite(L"TestResource/Player/Skill/Skill_Effect_Darkness_4_2.png");
		lockedEffect->GetAnimationBitmapComponent()->SliceSpriteSheet(550, 550, 0, 0, 0, 0);
		lockedEffect->GetAnimationBitmapComponent()->FrameResize(45);
		lockedEffect->GetAnimationBitmapComponent()->SetFrameDurations({ 1.0f / 45 });
		lockedEffect->GetAnimationBitmapComponent()->Trigger(false);
		lockedEffect->GetAnimationBitmapComponent()->Trigger(true);
		lockedEffect->GetAnimationBitmapComponent()->SetLoop(true);
		lockedEffect->SetAliveTime(1000000.f);
		lockedEffect->GetAnimationBitmapComponent()->SetOpacity(0.f);
		lockedEffect->SetLocation(player->GetLocation().x, player->GetLocation().y);
	}
}
