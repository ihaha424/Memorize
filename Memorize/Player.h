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
		return *this;
	}
};

class Player : public Character, public IReflection
{
	LOG_REGISTER_OBJ(Player)
public:

	float moveSpeed = 450;
	Stat stat;
	float basicAttackTime = 1.f;
	int skillUses;
public:
	Player(class World* _world);
	virtual ~Player();

	void AddSkillUses() { skillUses++; };
	int GetSkillUses() { return skillUses; }
	void AddToStat(Stat _addStat);
	Stat& GetStat() { return stat; }

	virtual void Update(float _dt) override;


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

