#include "PlayerBlinking.h"
#include "GPlayerController.h"
#include "Player.h"
#include "MovementComponent.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Mouse.h"
#include "../D2DGameEngine/BoxComponent.h"
#include "../D2DGameEngine/AnimationEffect.h"
#include "../D2DGameEngine/AnimationBitmapComponent.h"


void PlayerBlinking::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->GetPlayer()->box->SetCollisionEnabled(CollisionEnabled::NoCollision);
	
	//점멸 애니메이션
	curtime = blinkTime;
	D2D_Vec2F mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	worldMousePos = playerController->GetPlayer()->GetWorld()->ScreenToWorldPoint({ mousePos.x, mousePos.y });

	Math::Vector2 playerPos = playerController->GetPlayer()->GetLocation();

	playerController->GetPlayer()->GetComponent<MovementComponent>()->SetSpeed(0.f);

	//BlickSource Effect
	{
		AnimationEffect* BlickSource = playerController->GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		BlickSource->SetSprite(L"TestResource/Player/Effect/FlashSource.png");
		BlickSource->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		BlickSource->GetAnimationBitmapComponent()->SetFrameDurations({ 0.04f });
		BlickSource->GetAnimationBitmapComponent()->Trigger(true);
		BlickSource->SetAliveTime(1.f);
		BlickSource->SetLocation(playerPos.x, playerPos.y);
	}
	
	//Player Pos
	if ((worldMousePos - playerPos).Length() < blinkDistance)
	{
		playerPos = worldMousePos;
	}
	else
	{
		Math::Vector2 direction = worldMousePos - playerPos;
		direction.Normalize();
		playerPos = playerPos + (direction * blinkDistance);
	}
	playerPos = ClampPointInDiamond(playerPos);
	playerController->GetPlayer()->SetLocation(playerPos.x, playerPos.y);

	//BlickDest Effect
	{
		AnimationEffect* BlickDest = playerController->GetWorld()->GetEffectSystem().CreateEffect<AnimationEffect>();
		BlickDest->SetSprite(L"TestResource/Player/Effect/FlashDest.png");
		BlickDest->GetAnimationBitmapComponent()->SliceSpriteSheet(400, 500, 0, 0, 0, 0);
		BlickDest->GetAnimationBitmapComponent()->SetFrameDurations({ 0.041f });
		BlickDest->GetAnimationBitmapComponent()->Trigger(true);
		BlickDest->SetAliveTime(1.f);
		BlickDest->SetLocation(playerPos.x, playerPos.y);
	}

	SoundManager::PlayMusic(L"TestResource/Sound/Player/Sound_Flash.wav");
}

void PlayerBlinking::Update(float _dt)
{
	curtime -= _dt;
	if (curtime < 0.f)
	{
		owner->SetNextState(owner->GetPrevState());
	}
}

void PlayerBlinking::Exit()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	playerController->GetPlayer()->box->SetCollisionEnabled(CollisionEnabled::EnableCollision);
}

Math::Vector2 PlayerBlinking::ClampPointInDiamond(Math::Vector2 point)
{
	// 초기 값을 설정
	Math::Vector2 clampedPoint = point;

	if (positiveSlopeUp.WhichSide(clampedPoint) == -1)
	{
		clampedPoint = positiveSlopeUp.NearestPoint(clampedPoint);
	}
	if (positiveSlopeDwon.WhichSide(clampedPoint) == 1)
	{
		clampedPoint = positiveSlopeDwon.NearestPoint(clampedPoint);
	}
	if (negativeSlopeUp.WhichSide(clampedPoint) == -1)
	{
		clampedPoint = negativeSlopeUp.NearestPoint(clampedPoint);
	}
	if (negativeSlopeDown.WhichSide(clampedPoint) == 1)
	{
		clampedPoint = negativeSlopeDown.NearestPoint(clampedPoint);
	}

	return clampedPoint;
}
