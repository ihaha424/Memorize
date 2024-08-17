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
#include "../D2DGameEngine/CircleComponent.h"

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();
	skillUses = 100;
	SetTickProperties(TICK_PHYSICS | TICK_PRE_UPDATE | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);
	renderLayer = TestLevel1_RenderLayer::Object;
	
	// 애니메이션
	{
		Animator* abm = CreateComponent<Animator>();
		rootComponent->AddChild(abm);
		AnimationState* PlayerAnimationState;
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

	//주변 적을 감지하기 위한 원형 콜라이더 for waterball skill
	rangeCircle = CreateComponent <CircleComponent>();
	rootComponent->AddChild(rangeCircle);
	rangeCircle->SetCircleRadius(waterBallRange);
	rangeCircle->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	rangeCircle->bSimulatePhysics = false;
	rangeCircle->bApplyImpulseOnDamage = false;
	rangeCircle->bGenerateOverlapEvent = true;
}

Player::~Player()
{
}

void Player::AddToStat(Stat _addStat)
{
	stat = stat + _addStat;
}

void Player::PreUpdate(float _dt)
{
	__super::PreUpdate(_dt);
	//매 틱마다 범위 내에 있는 적을 감지하기 위해 초기화해줌
	//enemiesInRange.clear();
}

void Player::Update(float _dt)
{
	__super::Update(_dt);
	basicAttackTime -= _dt;
	stat.mp += _dt * 100;
}

void Player::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	OBJ_MESSAGE("Overlap began!");


	//TODO :: 매 틱마다 체크하는 것으로 바꿔야 함!!!!! 
	//범위 내에 있는 적을 체크하여 배열에 넣음 
	Character* ch = dynamic_cast<Character*>(other);
	if (ch) 
	{
		enemiesInRange.push_back(ch);
	}
	
}


void Player::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Reflection/PlayerStatus.txt");
	reflectionResource->ParsingFile(0, stat.maxHp, stat.maxMp, stat.hp, stat.mp, stat.defaultAttackSpeed, skillUses);
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

