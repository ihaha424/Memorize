#include "PlayerBlinking.h"
#include "GPlayerController.h"
#include "Player.h"
#include "../D2DGameEngine/World.h"
#include "../D2DGameEngine/Mouse.h"

void PlayerBlinking::Enter()
{
	GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
	//playerController->GetPlayer()->colliderOff
	//점멸 애니메이션
	curtime = blinkTime;
	D2D_Vec2F mousePos = { Mouse::curMousePosition.x, Mouse::curMousePosition.y };
	worldMousePos = playerController->GetPlayer()->GetWorld()->ScreenToWorldPoint({ mousePos.x, mousePos.y });
}

void PlayerBlinking::Update(float _dt)
{
	curtime -= _dt;
	if (curtime < 0.f)
	{
		GPlayerController* playerController = static_cast<GPlayerController*>(owner->GetOwner());
		Math::Vector2 playerPos = playerController->GetPlayer()->GetLocation();
		if ((worldMousePos - playerPos).Length() < blinkDistance)
		{
			playerController->GetPlayer()->SetLocation(worldMousePos.x, worldMousePos.y);
		}
		else
		{
			Math::Vector2 direction = worldMousePos - playerPos;
			direction.Normalize();
			playerPos = playerPos + (direction * blinkDistance);
			playerController->GetPlayer()->SetLocation(playerPos.x, playerPos.y);
		}
		owner->SetNextState(owner->GetPrevState());
	}
}

void PlayerBlinking::Exit()
{
	//Collider->Active
}
