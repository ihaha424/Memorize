#include "Player.h"
#include "D2DGameEngine/Animator.h"
//#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "PlayerAnimationStates.h"


#include "GCameraComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BoxComponent.h"
#include "TestLevel1_RenderLayer.h"
#include "MagicCircle.h"
#include "D2DGameEngine/DamageEvent.h"

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();

	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);
	renderLayer = TestLevel1_RenderLayer::Object;
	
	// 애니메이션
	{
		//Animator* abm = CreateComponent<Animator>();
		//rootComponent->AddChild(abm);
		//AnimationState* PlayerAnimationState;
		{
			//PlayerAnimationState = abm->CreateState<PlayerIdleAnimation>();
			//PlayerAnimationState->SetSprite(L"TestResource/Player/Orb/Orb.png");
			//PlayerAnimationState->SliceSpriteSheet(88, 100, 0, 0, 0, 0);
			//PlayerAnimationState->SetFrameDurations({ 0.05f });
			//PlayerAnimationState->Trigger(true);
			//abm->Initialize(PlayerAnimationState);

			//PlayerAnimationState = abm->CreateState<PlayerIdleAnimation>();
			//PlayerAnimationState->SetSprite(L"TestResource/Boss/MagicCircle/BossGrowCircle.png");
			//PlayerAnimationState->SliceSpriteSheet(650, 500, 0, 0, 0, 0);
			//PlayerAnimationState->SetFrameDurations({ 0.033f });
			//PlayerAnimationState->Trigger(true);
			//abm->Initialize(PlayerAnimationState);

			PlayerAnimationState = abm->CreateState<PlayerIdleAnimation>();
			PlayerAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerIdle.png");
			PlayerAnimationState->SliceSpriteSheet(137, 254, 0, 0, 0, 0);
			PlayerAnimationState->SetFrameDurations({ 0.1f });
			PlayerAnimationState->Trigger(true);
			abm->Initialize(PlayerAnimationState);

			PlayerAnimationState = abm->CreateState<PlayerMoveAnimation>();
			PlayerAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerMove.png");
			PlayerAnimationState->SliceSpriteSheet(162, 254, 0, 0, 0, 0);
			PlayerAnimationState->SetFrameDurations({ 0.08f });
			PlayerAnimationState->Trigger(true);

			//??abm->DeclareVariable<bool>("isMoving");
		}
	}

	/*AnimationBitmapComponent* abm = CreateComponent<AnimationBitmapComponent>();
	rootComponent = abm;
	{
		abm->SetSprite(L"TestResource/Player/PlayerMotions/PlayerMove.png");
		abm->SliceSpriteSheet(180, 216, 0,0,20,0);
	}
	abm->SetFrameDurations({0.05f});
	abm->Trigger(true);*/

	GCameraComponent* cm = CreateComponent<GCameraComponent>();
	GetWorld()->SetMainCamera(cm);
	rootComponent->AddChild(cm);

	MovementComponent* mv = CreateComponent< MovementComponent>();
	rootComponent->AddChild(mv);

	MagicCircle* mc = CreateComponent<MagicCircle>();
	rootComponent->AddChild(mc);
}

Player::~Player()
{
}

void Player::AddToStat(Stat _addStat)
{
	stat = stat + _addStat;
}

void Player::Update(float _dt)
{
	__super::Update(_dt);
	basicAttackTime -= _dt;
}

void Player::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PlayerStatus.txt");
	reflectionResource->ParsingFile(0, stat.maxHp, stat.maxMp, stat.hp, stat.mp, stat.defaultAttackSpeed, stat.skillUses);
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

