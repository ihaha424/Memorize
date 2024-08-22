#include "Scarecrow.h"
#include "Signal.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Canvas.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "Player.h"
#include "SkillActor.h"
#include "BossHitEffect.h"
#include "D2DGameEngine/RandomGenerator.h"
#include "D2DGameEngine/AnimationBitmapComponent.h"

Scarecrow::Scarecrow(World* _world) : Character(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	renderLayer = 3;

	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::Enemy);
	box->collisionProperty.responseContainer.SetAllChannels(CollisionResponse::Ignore);
	//box->collisionProperty.SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Block);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Overlap);
	box->collisionProperty.SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Overlap);
	box->SetBoxExtent({ 187 / 2.8f, 287 / 2.4f });
	hp = maxHp;

	OnHPChanged = new Signal<float>;

	box->SetCollisionObjectType(ECollisionChannel::Enemy);


	// 애니메이션
	{
		abm = CreateComponent<AnimationBitmapComponent>();
		rootComponent->AddChild(abm);
		abm->SetSprite(L"TestResource/Tutorial/dummy_monster.png");
		abm->SliceSpriteSheet(258, 234, 0, 0, 0, 0);
		abm->SetFrameDurations({ 1.f });

	}

	SetScale(2.f, 2.f);
	
}

Scarecrow::~Scarecrow()
{
	delete OnHPChanged;
}

void Scarecrow::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	OnHPChanged->Emit(hp / (float)maxHp);

	if (Periodic_Pattern_Cool_Time > 0.f)
		Periodic_Pattern_Cool_Time -= _dt;

	//Flip
	Math::Vector2 playerPos = GetWorld()->FindActorByType<Player>()->GetLocation();
	playerPos = GetLocation() - playerPos;
	if (playerPos.x < 0.f)
		abm->SetScale(-1.f, 1.f);
	else
		abm->SetScale(1.f, 1.f);

	if (tweenRotate)
	{
		tweenRotate->Update(_dt);
		SetRotation(rotateValue);
	}
		
}

void Scarecrow::OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult)
{
	OBJ_MESSAGE("Hit!");
}

void Scarecrow::OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser)
{
	hp -= damageAmount;	// 체력을 받은 데미지 만큼 감소시킵니다.

	SkillActor* temp = static_cast<SkillActor*>(damageCauser);
	BossHitEffect* bosshitEffect = GetWorld()->GetEffectSystem().CreateEffect<BossHitEffect>();
	bosshitEffect->SetEffect(temp->skill.element);
	Math::Vector2 thisPos = GetLocation();
	bosshitEffect->SetLocation(thisPos.x + Random::Get((int)200) - 100, thisPos.y + Random::Get((int)500) - 250);
	if (hp <= 0.f)	// 만약 체력이 0보다 작거나 같다면,
	{
		hp = 0.f;
		//TODO 사망 시 처리 
	}

	tweenRotate = new DotTween<float>;
	tweenRotate->SetData(&rotateValue);
	tweenRotate->SetDuration(1);
	tweenRotate->SetStartPoint(-2);
	tweenRotate->SetEndPoint(2);
	tweenRotate->SetEasingEffect(EasingEffect::Linear);
	tweenRotate->SetStepAnimation(StepAnimation::StepOncePingPong);
	
}