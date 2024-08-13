#pragma once
#include "../D2DGameEngine/Character.h"
#include "IBossSkill.h"

class Boss : public Character
{
public:
	int hp;
	int maxHp = 100;
	int DissfellCount = 0;
	float Periodic_Pattern_Cool_Time = 0.f;		//메인 패턴의 딜레이
	float Phase_Pattern_Cool_Time = 0.f;		//각 페이즈의 특수 패턴 딜레이
	float Pattern_Delay = 0.f;					//각 패턴의 딜레이
	float Detection_Range = 880.f;
	float Avoidance_Range = 450.f;



public:
	Boss(class World* _world);
	virtual ~Boss();

	virtual void Update(float _dt) override;

};

