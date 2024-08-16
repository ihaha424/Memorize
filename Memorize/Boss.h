#pragma once
#include "../D2DGameEngine/Character.h"

#include "D2DGameEngine/Debug.h"

class Boss : public Character
{
	LOG_REGISTER_OBJ(Boss)

public:
	int hp;
	int maxHp = 100;
	int DissfellCount = 0;
	float Periodic_Pattern_Cool_Time = 0.f;		//���� ������ ������
	float Phase_Pattern_Cool_Time = 0.f;		//�� �������� Ư�� ���� ������
	float Pattern_Delay = 0.f;					//�� ������ ������
	float Detection_Range = 880.f;
	float Avoidance_Range = 450.f;


	class BossHPPanel* hpBar;
public:
	Boss(class World* _world);
	virtual ~Boss();

	virtual void Update(float _dt) override;

	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) override {
		OBJ_MESSAGE("Hit!");
	}

	virtual void OnBeginOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap began!");
	}

	virtual void OnEndOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap ended!");
	}



};

