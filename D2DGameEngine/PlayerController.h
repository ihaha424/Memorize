#pragma once
#include "Controller.h"

class PlayerController : public Controller
{
protected:
	class InputComponent* inputComponent;

public:
	PlayerController(class World* _world);
	virtual ~PlayerController();
	virtual void SetupInputComponent();
};

