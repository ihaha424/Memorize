#pragma once
#include "Controller.h"

class PlayerController : public Controller
{
	class InputComponent* inputComponent;

public:
	virtual void SetupInputComponent();
};

