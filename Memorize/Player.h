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
	float hpRegenPerSecond; //�ʴ� HP ȸ����
	float mpRegenPerSecond; //�ʴ� MP ȸ����
	float skillRange; //���� ����
	int castingSpeed; //ĳ���� �ӵ�
	int numProjectiles; //����ü ����
	int defaultDamage; //�⺻ �����
	int defaultAttackSpeed; //�⺻ ���� �ӵ�
	bool manaOverLoad = false;
	int skillUses;

	Stat operator+(Stat stat)
	{
		Stat ret;
		ret.hp += stat.hp;
		ret.maxHp += stat.maxHp;
		ret.mp += stat.mp;
		ret.maxMp += stat.maxMp;
		ret.hpRegenPerSecond += stat.hpRegenPerSecond;
		ret.mpRegenPerSecond += stat.mpRegenPerSecond;
		ret.skillRange += stat.skillRange;
		ret.castingSpeed += stat.castingSpeed;
		ret.numProjectiles += stat.numProjectiles;
		ret.defaultDamage += stat.defaultDamage;
		ret.defaultAttackSpeed += stat.defaultAttackSpeed;
		ret.manaOverLoad = stat.manaOverLoad;
		ret.skillUses = stat.skillUses;
		return ret;
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
		skillUses *= -1;
		return *this;
	}
};

class Player : public Character, IReflection
{
	LOG_REGISTER_OBJ(Player)
	const int maxLevel = 50;
	int level = 1;
	int exp = 0;
	float moveSpeed = 100;
	Stat stat;

public:
	Player(class World* _world);
	virtual ~Player();

	void AddSkillUses() { stat.skillUses++; };
	int GetSkillUses() { return stat.skillUses; }
	void AddToStat(Stat _addStat);
	Stat& GetStat() { return stat; }

	// IReflection��(��) ���� ��ӵ�
	void ReflectionIn() override;
	void ReflectionOut() override;

	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) override {
		OBJ_MESSAGE("Hit!");
	}

	virtual void OnBeginOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap began!");
	}

	virtual void OnEndOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap ended!");
	}

	void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) override
	{
		OBJ_MESSAGE(dbg::text(damageAmount, " damage taken!"));
	}
};

