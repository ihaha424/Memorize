#pragma once
#include "../D2DGameEngine/PlayerController.h"

class TestPlayerController : public PlayerController
{
public:
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	void MovePlayer();
};

