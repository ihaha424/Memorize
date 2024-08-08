#pragma once
#include "../D2DGameEngine/PlayerController.h"

class GPlayerController : public PlayerController
{
public:
	GPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	void MovePlayer();
};

