#pragma once
#include "../D2DGameEngine/Character.h"
#include "../D2DGameEngine/Reflection.h"


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

class Player : public Character, IReflection
{
	const int maxLevel = 50;
	int level = 1;
	int exp = 0;
	float moveSpeed = 100;
	int	ReflectionData = 100;
	int	ReflectionData1 = 100;
	int	ReflectionData2 = 100;
	std::vector<float> ReflectionData3 = {};
	Stat stat;
public:
	Player(class World* _world);
	virtual ~Player();

	void LevelUp();


	// IReflection을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

};

