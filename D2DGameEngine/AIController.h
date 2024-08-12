#pragma once
#include "Controller.h"

class BehaviorTree;
class BrainComponent;

class AIController : public Controller
{
protected:
	BrainComponent* brainComponent;

public:
	AIController(class World* _world);
	virtual ~AIController();

	BrainComponent* GetBrainComponent() const {
		return brainComponent;
	}

	// TODO:
	virtual bool MoveToLocation(Math::Vector2 location) {
		// Check the CSpace and see if the location is available

		// Query move

		return false;
	}

	// TODO:
	virtual bool TeleportToLocation(Math::Vector2 location) {
		// Check the CSpace and see if the location is available

		// Query teleport

		return false;
	}

	// TODO:
	virtual bool RunBehaviorTree(BehaviorTree* _bt) {
	
		return false;
	}

	void Update(float _dt);
};

