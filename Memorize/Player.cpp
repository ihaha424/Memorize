#include "Player.h"
//#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "PlayerAnimationStates.h"


#include "GCameraComponent.h"
#include "MovementComponent.h"
#include "D2DGameEngine/World.h"
#include "D2DGameEngine/ReflectionResource.h"
#include "D2DGameEngine/ResourceManager.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/Mouse.h"
#include "TestLevel1_RenderLayer.h"
#include "MagicCircle.h"
#include "BuffEffectComponent.h"
#include "ManaOverloadComponent.h"
#include "MemorizeEffect.h"
#include "ElementalMasterComponent.h"
#include "D2DGameEngine/DamageEvent.h"
#include "../D2DGameEngine/CircleComponent.h"

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();
	skillUses = 100;
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);
	renderLayer = TestLevel1_RenderLayer::Object;

	GetComponent<BoxComponent>()->SetCollisionObjectType(ECollisionChannel::Player);
	
	OnHPChanged = new Signal<float>;
	OnMPChanged = new Signal<float>;

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

void Player::StartMemorizeEffect()
{
	MemorizeEffect* memorize = GetWorld()->GetEffectSystem().CreateEffect<MemorizeEffect>();
	memorize->StartEffect();
	memorize->SetPosition(&memorizePos);
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

	OnHPChanged->Emit(stat.hp / (float)stat.maxHp);
	OnMPChanged->Emit(stat.mp / (float)stat.maxMp);



	//Orb
	Math::Vector2 mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	mousePos = GetWorld()->ScreenToWorldPoint(mousePos);
	Math::Vector2 direction = mousePos - Math::Vector2(GetLocation().x, GetLocation().y);
	direction.Normalize();

	orb->SetTranslation(100 * direction.x, direction.y * 5);

	memorizePos = { GetLocation().x, GetLocation().y - 230 };
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

void Player::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	stat.hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	if (stat.hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		stat.hp = 0.f;
		//TODO 사망 시 처리 
	}
}