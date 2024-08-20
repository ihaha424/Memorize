#pragma once
#include "../D2DGameEngine/Character.h"
#include "../D2DGameEngine/Reflection.h"
#include "D2DGameEngine/Debug.h"
#include "Signal.h"

struct Stat
{
	float hp;
	float maxHp; //HP
	float mp;
	float maxMp = 100.f; //MP
	float hpRegenPerSecond; //�ʴ� HP ȸ����
	float mpRegenPerSecond = 20; //�ʴ� MP ȸ����
	float skillRange; //���� ����
	int castingSpeed; //ĳ���� �ӵ�
	int numProjectiles; //����ü ����
	int defaultDamage; //�⺻ �����
	float defaultAttackSpeed = 1.f; //�⺻ ���� �ӵ�
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
	float minMp = 0.f;
	float minHp = 0.f;
	float minMaxMp = 0.f;
	float maxMaxMp = 1000.f;
	float minAttackSpeed = 1.f;
	float maxAttackSpeed = 2.f;

	class AnimationBitmapComponent* orb;
public:

	float moveSpeed = 450;
	Stat stat;
	float basicAttackTime = 1.f;
	int skillUses;
	bool bondageFlag{ false };

	class BuffEffectComponent* buffEffect;
	class GCameraComponent* cm;
	class ManaOverloadComponent* manaOverloadEffect;
	class MovementComponent* mv;
	class Animator* abm;
	class AnimationState* IdleAnimationState;
	class AnimationState* MoveAnimationState;
	class AnimationState* DieAnimationState;
	
	Math::Vector2 headEffectPos;
public:
	Player(class World* _world);
	virtual ~Player();

	void StartHeadEffect(int index);
	void AddSkillUses() { skillUses++; };
	int GetSkillUses() { return skillUses; }
	void AddToStat(Stat _addStat);
	Stat& GetStat() { return stat; }

	virtual void Update(float _dt) override;
	virtual void Render(class D2DRenderer* _renderer) override;

	// IReflection��(��) ���� ��ӵ�
	void ReflectionIn() override;
	void ReflectionOut() override;

	virtual void OnHit(PrimitiveComponent* myComp, PrimitiveComponent* otherComp, bool bSelfMoved, const HitResult& hitResult) override {
		OBJ_MESSAGE("Hit!");
	}

	virtual void OnEndOverlap(Actor* other) {
		OBJ_MESSAGE("Overlap ended!");
	}


	virtual float InternalTakeDamage(float damageAmount, DamageEvent const& damageEvent, Controller* eventInstigator, Actor* damageCauser) override;

	void OnTakeDamage(float damageAmount, struct DamageEvent const& damageEvent, class Controller* eventInstigator, Actor* damageCauser) override;


	//Signals for UI
	Signal<float>* OnHPChanged;
	Signal<float>* OnMPChanged;

};

