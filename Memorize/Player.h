#pragma once
#include "../D2DGameEngine/Character.h"


struct Stat
{
	float maxHp; //HP
	float maxMp; //MP
	float hpRegenPerSecond; //초당 HP 회복량
	float mpRegenPerSecond; //초당 MP 회복량
	float skillRange; //시전 범위
	int castingSpeed; //캐스팅 속도
	int numProjectiles; //투사체 개수
	int defaultDamage; //기본 대미지
};

class Player : public Character
{
	int level = 1;
	Stat stat;
public:
	Player(class World* _world);
	virtual ~Player();

};

