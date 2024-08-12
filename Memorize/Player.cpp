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

Player::Player(class World* _world) : Character(_world)
{
	ReflectionIn();

	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER | TICK_POST_UPDATE);

	// NOTE: Test collision

	BoxComponent* box = CreateComponent<BoxComponent>();
	box->collisionProperty = CollisionProperty(CollsionPropertyPreset::BlockAll);
	box->bSimulatePhysics = true;
	box->bApplyImpulseOnDamage = false;
	box->bGenerateOverlapEvent = true;
	box->InitBoxExtent({ 124, 220 });
	rootComponent = box;

	// �ִϸ��̼�
	{
		Animator* abm = CreateComponent<Animator>();
		rootComponent->AddChild(abm);
		AnimationState* PlayerAnimationState;
		{
			PlayerAnimationState = abm->CreateState<PlayerIdleAnimation>();
			PlayerAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerIdle.png");
			PlayerAnimationState->SliceSpriteSheet(120, 216, 0, 0, 20, 0);
			PlayerAnimationState->SetFrameDurations({ 0.05f });
			PlayerAnimationState->Trigger(true);
			abm->Initialize(PlayerAnimationState);

			PlayerAnimationState = abm->CreateState<PlayerMoveAnimation>();
			PlayerAnimationState->SetSprite(L"TestResource/Player/PlayerMotions/PlayerMove.png");
			PlayerAnimationState->SliceSpriteSheet(180, 216, 0, 0, 20, 0);
			PlayerAnimationState->SetFrameDurations({ 0.05f });
			PlayerAnimationState->Trigger(true);

			//??abm->DeclareVariable<bool>("isMoving");
		}
	}

	//AnimationBitmapComponent* abm = CreateComponent<AnimationBitmapComponent>();
	//rootComponent = abm;
	//{
	//	abm->SetSprite(L"TestResource/Player/PlayerMotions/PlayerMove.png");
	//	abm->SliceSpriteSheet(180, 216, 0,0,20,0);
	//}
	//abm->SetFrameDurations({0.05f});
	//abm->Trigger(true);

	GCameraComponent* cm = CreateComponent<GCameraComponent>();
	GetWorld()->SetMainCamera(cm);
	rootComponent->AddChild(cm);

	MovementComponent* mv = CreateComponent< MovementComponent>();
	rootComponent->AddChild(mv);
}

Player::~Player()
{
}

void Player::LevelUp()
{
	if (level < maxLevel)
	{
		level++;
		exp = 0;
	}

	//TODO
	//level�� ���� Stat ����
}

void Player::AddToStat(Stat _addStat)
{
	stat = stat + _addStat;
}

void Player::ReflectionIn()
{
	std::shared_ptr<ReflectionResource> reflectionResource = ResourceManager::LoadResource<ReflectionResource>(L"TestResource/Player.txt");
	reflectionResource->ParsingFile(0, moveSpeed);
}

void Player::ReflectionOut()
{}
