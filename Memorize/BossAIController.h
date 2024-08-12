#pragma once
#include "../D2DGameEngine/AIController.h"
class BossAIController : public AIController
{
private:
	class Boss* boss;
public:
	BossAIController(class World* _world);
	virtual ~BossAIController();

	virtual void Update(float  _dt) override;

	void InitializeBoss();

	class Boss* GetBoss() { return boss; }
	void SetBoss(class Boss* _boss) { boss = _boss; }

};

