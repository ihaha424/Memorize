#pragma once
#include "Controller.h"

class PlayerController : public Controller
{
protected:
	class InputComponent* inputComponent;
	class Player* player;
public:
	PlayerController(class World* _world);
	virtual ~PlayerController();
	virtual void SetupInputComponent();
	class Player* GetPlayer() { return player; }
	void SetPlayer(class Player* _player) { player = _player; }
};

