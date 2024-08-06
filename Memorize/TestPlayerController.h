#pragma once
#include "../D2DGameEngine/PlayerController.h"
#include "../D2DGameEngine/Debug.h"

class TestPlayerController : public PlayerController
{
	LOG_REGISTER_OBJ(TestPlayerController)
	Math::Vector2 destPos;
public:
	TestPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;
	void MovePlayer();
};

