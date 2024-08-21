﻿#include "Player.h"


#include "GCameraComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Mouse.h"
#include "TestLevel1_RenderLayer.h"
#include "MagicCircle.h"
#include "BuffEffectComponent.h"
#include "ManaOverloadComponent.h"
#include "HeadEffect.h"
#include "ElementalMasterComponent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "../D2DGameEngine/CircleComponent.h"

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();
	skillUses = 200;
	stat.defaultDamage = 20;
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);
	renderLayer = TestLevel1_RenderLayer::Object;

	collisionBox = GetComponent<BoxComponent>(); 
	collisionBox->SetCollisionObjectType(ECollisionChannel::Player);

	OnHPInfoChanged = new Signal<float, float>;
	OnMPInfoChanged = new Signal<float, float>;

	// ¾Ö´Ï¸ÞÀÌ¼Ç
	{
		abm = CreateComponent<Animator>();
		rootComponent->AddChild(abm);
		{
			IdleAnimationState = abm->CreateState<AnimationState>();
			IdleAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerIdle.png");
			IdleAnimationState->SliceSpriteSheet(137, 254, 0, 0, 0, 0);
			IdleAnimationState->SetFrameDurations({ 0.1f });
			IdleAnimationState->Trigger(true);
			abm->Initialize(IdleAnimationState);
			
			MoveAnimationState = abm->CreateState<AnimationState>();
			MoveAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerMove.png");
			MoveAnimationState->SliceSpriteSheet(162, 254, 0, 0, 0, 0);
			MoveAnimationState->SetFrameDurations({ 0.08f });
			MoveAnimationState->Trigger(true);

			DieAnimationState = abm->CreateState<AnimationState>();
			DieAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerDie.png");
			DieAnimationState->SliceSpriteSheet(150, 254, 0, 0, 0, 0);
			DieAnimationState->SetFrameDurations({ 0.08f });
			DieAnimationState->Trigger(true);
		}
	}

	cm = CreateComponent<GCameraComponent>();
	GetWorld()->SetMainCamera(cm);
	rootComponent->AddChild(cm);

	mv = CreateComponent< MovementComponent>();
	rootComponent->AddChild(mv);

	orb = CreateComponent< AnimationBitmapComponent>();
	orb->SetSprite(L"TestResource/Player/Orb/Orb.png");
	orb->SliceSpriteSheet(88, 100, 0 ,0, 0, 0 );
	orb->SetFrameDurations({ 1.f / 13 });
	orb->Trigger(true);
	orb->SetLoop(true);
	rootComponent->AddChild(orb);

	MagicCircle* mc = CreateComponent<MagicCircle>();
	rootComponent->AddChild(mc);

	buffEffect = CreateComponent<BuffEffectComponent>();
	rootComponent->AddChild(buffEffect);
	buffEffect->SetStatus(OS_INACTIVE);

	manaOverloadEffect = CreateComponent<ManaOverloadComponent>();
	rootComponent->AddChild(manaOverloadEffect);
	manaOverloadEffect->SetStatus(OS_INACTIVE);

	
}

Player::~Player()
{
}

void Player::StartHeadEffect(int index)
{
	HeadEffect* effect = GetWorld()->GetEffectSystem().CreateEffect<HeadEffect>();
	effect->SetEffect(index);
	effect->SetPosition(&headEffectPos);
}

void Player::AddToStat(Stat _addStat)
{
	stat = stat + _addStat;

	stat.mp = std::clamp(stat.mp, minMp, stat.maxMp);
	stat.hp = std::clamp(stat.hp, minHp, stat.maxHp);
	stat.defaultAttackSpeed = std::clamp(stat.defaultAttackSpeed, minAttackSpeed, maxAttackSpeed);
	stat.maxMp = std::clamp(stat.maxMp, minMaxMp, maxMaxMp);
}



void Player::Update(float _dt)
{
	__super::Update(_dt);

	stat.mp += stat.mpRegenPerSecond * _dt;

	stat.maxMp = maxMaxMp;
	stat.mp = std::clamp(stat.mp, minMp, stat.maxMp);
	//std::cout << "MP:" << stat.mp << ", HP:" << stat.hp << std::endl;

	basicAttackTime -= stat.defaultAttackSpeed * _dt ;

	OnHPInfoChanged->Emit(stat.hp, stat.maxHp);
	OnMPInfoChanged->Emit(stat.mp, stat.maxMp);

	//Orb
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	Math::Vector2 direction = mousePos - Math::Vector2(GetLocation().x, GetLocation().y);
	direction.Normalize();

	orb->SetTranslation(100 * direction.x, direction.y * 5);

	//Flip
	if (direction.x < 0.f)
		abm->SetScale(-1.f, 1.f);
	else
		abm->SetScale(1.f, 1.f);

	//Animation
	if (stat.hp > 0.f)
	{
		if (mv->GetSpeed() < 10.f)
		{
			if(abm->GetCurrentAnimationScene() != IdleAnimationState)
				abm->SetState(IdleAnimationState);
		}
		else
		{
			if (abm->GetCurrentAnimationScene() != MoveAnimationState)
				abm->SetState(MoveAnimationState);
		}
	}
	else
	{
		if (abm->GetCurrentAnimationScene() != DieAnimationState)
			abm->SetState(DieAnimationState);
	}

	headEffectPos = { GetLocation().x, GetLocation().y - 230 };

	// 속박이면 속도 0
	if (bondageFlag)
	{
		SetVelocity({ 0, 0 });
	}
}

void Player::Render(D2DRenderer* _renderer)
{
	__super::Render(_renderer);
}


void Player::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PlayerStatus.txt");
	reflectionResource->ParsingFile(0, stat.maxHp, stat.maxMp, stat.hp, stat.mp, moveSpeed, skillUses);
}

void Player::ReflectionOut()
{}

float Player::InternalTakeDamage(float damageAmount, DamageEvent const& damageEvent, Controller * eventInstigator, Actor * damageCauser)
{
 	if (damageEvent.GetDamageEventType() == DamageEventType::RadialDamage)
	{
		const RadialDamageEvent& radialDamageEvent = static_cast<const RadialDamageEvent&>(damageEvent);
		const RadialDamageInfo& damageInfo = radialDamageEvent.radialDamageInfo;
		return damageInfo.GetDamageScale((GetLocation() - damageCauser->GetLocation()).Length());
	}
	return damageAmount;
}

void Player::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	// 피격
	cm->Trigger피격();

	stat.hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	if (stat.hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		stat.hp = 0.f;
		//TODO 사망 시 처리
	}
}