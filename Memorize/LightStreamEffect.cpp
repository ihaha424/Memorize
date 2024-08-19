#include "LightStreamEffect.h"
#include "D2DGameEngine/BoxComponent.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/DamageEvent.h"

LightStreamEffect::LightStreamEffect(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);
	rootComponent = anim = CreateComponent<Animator>();
	initialState = anim->CreateState<AnimationState>();
	normalState = anim->CreateState<AnimationState>();
	endingState = anim->CreateState<AnimationState>();

	initialState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream01.png");
	initialState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	initialState->FrameResize(25);
	initialState->SetFrameDurations({ 2.f / 25 });

	normalState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream02.png");
	normalState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	normalState->FrameResize(60);
	normalState->SetFrameDurations({ 5.f / 60 });

	endingState->SetSprite(L"TestResource/Player/Skill/Skill_LightStream03.png");
	endingState->SliceSpriteSheet(1000, 290, 0, 0, 0, 0);
	endingState->FrameResize(15);
	endingState->SetFrameDurations({ 1.f / 15 });

	anim->Initialize(initialState);

	box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollisionPropertyPreset::OverlapAll);
	box->bSimulatePhysics = false;	// �����ӿ� ������ �������� �ʽ��ϴ�.
	box->bApplyImpulseOnDamage = false;	// �������� ���� �� ����� ���մϴ�.
	box->bGenerateOverlapEvent = true;	// Overlap �̺�Ʈ�� �߻���ŵ�ϴ�.
	rootComponent->AddChild(box);
}

void LightStreamEffect::Initialize()
{
	state = State::Initial;
	anim->SetState(initialState);
	elapsedTime = 0.f;
}

void LightStreamEffect::BeginPlay()
{
	__super::BeginPlay();
	Inactivate();
}

void LightStreamEffect::OnBeginOverlap(Actor* other, const OverlapInfo& overlap)
{
	__super::OnBeginOverlap(other, overlap);

	if (box->bGenerateOverlapEvent == false)
		return;

	//������� ����
	DamageEvent damageEvent;
	DamageType damageType{
		.damageImpulse = 10000.f, //��ݷ� �־��ִ� �� �´���?
	};
	damageEvent.SetDamageType(damageType);

	other->TakeDamage(damage, damageEvent, nullptr, this);
}

void LightStreamEffect::Update(float _dt)
{
	__super::Update(_dt);

	elapsedTime += _dt;

	if (state == State::Normal)
	{
		box->bShouldOverlapTest = true;
	}

	if (elapsedTime > initialTime && state == State::Initial)
	{
		state = State::Normal;
		anim->SetState(normalState);
	}
	else if (elapsedTime >= initialTime + duration && state == State::Normal)
	{
		state = State::Ending;
		anim->SetState(endingState);
	}
	if (elapsedTime >= initialTime + duration + endingTime)
	{
		Inactivate();
	}
}

LightStreamEffect::~LightStreamEffect()
{
}
