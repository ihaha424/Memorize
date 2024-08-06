#pragma once
#include "Controller.h"

class PlayerController : public Controller
{
protected:
	class InputComponent* inputComponent;

public:
	virtual void SetupInputComponent();
};

