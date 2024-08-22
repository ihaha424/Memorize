#include "BossMeteoCircle.h"
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
#include "BossMeteo.h"

#include "CreatePurificationEffect.h"

BossMeteoCircle::BossMeteoCircle(World* _world)
	:BossSkillActor(_world)
{
	ReflectionIn();

	abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	bm->isVisible = false;
	abm->MarkBoundsDirty();

	disfellCommandCount = 2;
	CreateDisfellCommand();
	CreateComponent<ClickComponent>();

	coolTime = new CoolTime<void>(
		[this]() {
			BossMeteo* temp;
			temp = GetWorld()->GetCurLevel()->CreateActor<BossMeteo>();
			auto position = GetLocation();
			int offsetPos = Random::Get<int>(100)- 50;
			position.y += -540.f;
			position.y += -350.f;
			position.x += offsetPos;
			temp->SetPosAndDerection(position, player->GetLocation());

		},
		0.5f,
		true
	);

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

BossMeteoCircle::~BossMeteoCircle()
{}

void BossMeteoCircle::BeginPlay()
{
	__super::BeginPlay();

	
}

void BossMeteoCircle::Update(float _dt)
{
	__super::Update(_dt);
	dispelTime -= _dt;

	if (dispelTime > skillDuration)
		return;

	if (dispelTime >= 0.f)
	{
		// 매 프레임마다 CoolTime 업데이트
		coolTime->Update(_dt);
	}


	if (dispelTime < 0.f)
	{
		EventBus::GetInstance().PushEvent<DisFellEvent>(this, true);
		EventBus::GetInstance().DispatchEvent<DisFellEvent>();
		Destroy();
	}
}

bool BossMeteoCircle::Destroy()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);
	delete coolTime;

	return __super::Destroy();
}

void BossMeteoCircle::DisfellAction()
{
	Boss* boss = GetWorld()->FindActorByType<Boss>();
	Animator* abm = boss->abm;
	AnimationState* IdleAnimationState = boss->IdleAnimationState;
	AnimationState* CastingAnimationState = boss->CastingAnimationState;
	if (abm->GetCurrentAnimationScene() == CastingAnimationState)
		abm->SetState(IdleAnimationState);

	CreatePurificationEffect(GetWorld(), GetLocation(), 1.4f);

	Destroy();
}

void BossMeteoCircle::DisfellFailAction()
{
	disfellCommand.clear();
	dissfellindex = 0;
	CreateDisfellCommand();
}

void BossMeteoCircle::OnClicked()
{
	__super::OnClicked();
	EventBus::GetInstance().PushEvent<DisFellEvent>(this, false);
	EventBus::GetInstance().DispatchEvent<DisFellEvent>();
}

void BossMeteoCircle::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PatternData/Pattern13_Actor.txt");
	reflectionResource->ParsingFile(0, isDispel, dispelTime, skillDuration, damage);
}

void BossMeteoCircle::ReflectionOut()
{
}
