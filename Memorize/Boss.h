#pragma once
#include "../D2DGameEngine/Character.h"
#include "IBossSkill.h"

class Boss : public Character
{
public:
	int hp;
	int maxHp = 100;
	int DissfellCount = 0;
	float Periodic_Pattern_Cool_Time = 0.f;		//���� ������ ������
	float Phase_Pattern_Cool_Time = 0.f;		//�� �������� Ư�� ���� ������
	float Pattern_Delay = 0.f;					//�� ������ ������
	float Detection_Range = 880.f;
	float Avoidance_Range = 450.f;



public:
	Boss(class World* _world);
	virtual ~Boss();

	virtual void Update(float _dt) override;

};

