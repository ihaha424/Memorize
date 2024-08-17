#pragma once
#include "../D2DGameEngine/Character.h"
#include "../D2DGameEngine/Reflection.h"
#include "D2DGameEngine/Debug.h"

struct Stat
{
	float hp;
	float maxHp; //HP
	float mp;
	float maxMp; //MP
	float hpRegenPerSecond; //초당 HP 회복량
	float mpRegenPerSecond = 20; //초당 MP 회복량
	float skillRange; //시전 범위
	int castingSpeed; //캐스팅 속도
	int numProjectiles; //투사체 개수
	int defaultDamage; //기본 대미지
	int defaultAttackSpeed; //기본 공격 속도
	bool manaOverLoad = false;

	Stat operator+(Stat stat)
	{
		hp += stat.hp;
		maxHp += stat.maxHp;
		mp += stat.mp;
		maxMp += stat.maxMp;
		hpRegenPerSecond += stat.hpRegenPerSecond;
		mpRegenPerSecond += stat.mpRegenPerSecond;
		skillRange += stat.skillRange;
		castingSpeed += stat.castingSpeed;
		numProjectiles += stat.numProjectiles;
		defaultDamage += stat.defaultDamage;
		defaultAttackSpeed += stat.defaultAttackSpeed;
		manaOverLoad = stat.manaOverLoad;
		return *this;
	}
	Stat operator-()
	{
		hp *= -1;
		maxHp *= -1;
		mp *= -1;
		maxMp *= -1;
		hpRegenPerSecond *= -1;
		mpRegenPerSecond *= -1;
		skillRange *= -1;
		castingSpeed *= -1;
		numProjectiles *= -1; 
		defaultDamage *= -1;
		defaultAttackSpeed *= -1;
		return *this;
	}
};

class Player : public Character, public IReflection
{
	LOG_REGISTER_OBJ(Player)
private:
	float mpTimer = 0.f;

public:

	float moveSpeed = 450;
	Stat stat;
	float basicAttackTime = 1.f;
	int skillUses;

	float waterBallRange = 500.f;
	class CircleComponent* rangeCircle;
	class BuffEffectComponent* buffEffect;
	std::vector<class Character*> enemiesInRange; //현재 범위 내에 있는 적

public:
	Player(class World* _world);
	virtual ~Player();

	void AddSkillUses() { skillUses++; };
	int GetSkillUses() { return skillUses; }
	void AddToStat(Stat _addStat);
	Stat& GetStat() { return stat; }

	virtual void PostUpdate(float _dt) override;
	virtual void Update(float _dt) override;

	virtual void OnOverlap(Actor* other, const OverlapInfo& overlap) override;

	// IReflection을(를) 통해 상속됨
	void ReflectionIn() override;
	void ReflectionOut() override;

	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) override {
		OBJ_MESSAGE("Hit!");
	}

	virtual void OnEndOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap ended!");
	}


	virtual float InternalTakeDamage(float damageAmount, DamageEvent const& damageEvent, Controller* eventInstigator, Actor* damageCauser) override;

	void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) override
	{
		OBJ_MESSAGE(dbg::text(damageAmount, " damage taken!"));
	}

};

