#pragma once
#include "Controller.h"

class BehaviorTree;

class AIController : public Controller
{
	BehaviorTree* bt = nullptr;
public:
	AIController(class World* _world);
	virtual ~AIController();

};

