#include "BossThroughProjectileMagicCircle.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"
#include "../D2DGameEngine/ClickComponent.h"
#include "../D2DGameEngine/EventBus.h"
#include "D2DGameEngine/RandomGenerator.h"

#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Boss.h"

#include "D2DGameEngine/World.h"
#include "Player.h"
#include "DisfellEvent.h"
#include "BossThroughProjectile.h"

BossThroughProjectileMagicCircle::BossThroughProjectileMagicCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	renderLayer = 2;

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	bm->isVisible = false;
	abm->MarkBoundsDirty();

	disfellCommandCount = 2;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	coolTime = new CoolTime<void>(
		[this]() {
			BossThroughProjectile* temp;
			temp = GetWorld()->GetCurLevel()->CreateActor<BossThroughProjectile>();
			auto position = GetLocation();
			//시전 방향 기준 5도 간격 9방향 체크 
			Math::Vector2 direction = player->GetLocation() - position;
			int directionInt = Random::Get<int>(17) - 9;
			float radians = directionInt * 5 * PI / 180.f;
			Math::Vector2 newDir =
			{
				direction.x* std::cos(radians) - direction.y * std::sin(radians),
				direction.x* std::sin(radians) + direction.y * std::cos(radians)
			};
			newDir.Normalize();
			temp->SetPosAndDerection(position, newDir);

		},
		0.2f,
		true
	);
}

BossThroughProjectileMagicCircle::~BossThroughProjectileMagicCircle()
{
	delete coolTime;
}

void BossThroughProjectileMagicCircle::BeginPlay()
{
	__super::BeginPlay();

	{
		abm->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowMagicCircle.png");
		abm->SliceSpriteSheet(1200, 1200, 0, 0, 0, 0);
		abm->SetFrameDurations({ 0.05f });
		abm->FrameResize(82);
		abm->SetLoop(true);
		abm->Trigger(true);
	}
	abm->SetScale(0.5f, 0.5f);

	player = GetWorld()->FindActorByType<Player>();
}

void BossThroughProjectileMagicCircle::Update(float _dt)
{
	__super::Update(_dt);
	skillDuration -= _dt;
	
	if (skillDuration >= 0.f)
	{
		// 매 프레임마다 CoolTime 업데이트
		coolTime->Update(_dt);
	}

	
	if (skillDuration < 0.f)
	{
		std::cout << "Finish\n";
		Boss* boss = GetWorld()->FindActorByType<Boss>();
		Animator* abm = boss->abm;
		AnimationState* IdleAnimationState = boss->IdleAnimationState;
		AnimationState* CastingAnimationState = boss->CastingAnimationState;
		if (abm->GetCurrentAnimationScene() == CastingAnimationState)
			abm->SetState(IdleAnimationState);
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		Destroy();
	}
}

void BossThroughProjectileMagicCircle::DisfellAction()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);
	Destroy();
}

void BossThroughProjectileMagicCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossThroughProjectileMagicCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossThroughProjectileMagicCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern09_Actor.txt");
	reflectionResource->ParsingFile(0, dispelTime, skillDuration, damage, speed, duration);
}

void BossThroughProjectileMagicCircle::ReflectionOut()
{
}
