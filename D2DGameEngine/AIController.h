#pragma once
#include "Controller.h"

class BehaviorTree;

class AIController : public Controller
{
protected:
	BehaviorTree* bt = nullptr;
public:
	AIController(class World* _world);
	virtual ~AIController();

	void Update(float _dt);
};

