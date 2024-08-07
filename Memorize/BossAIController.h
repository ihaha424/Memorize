#pragma once
#include "../D2DGameEngine/AIController.h"
class BossAIController : public AIController
{
public:
	BossAIController(class World* _world);
	virtual ~BossAIController();

	virtual void Update(float  _dt) override;

};

