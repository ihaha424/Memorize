#pragma once
#include "../D2DGameEngine/PlayerController.h"

class TestPlayerController : public PlayerController
{
public:
	TestPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	void MovePlayer();
};

