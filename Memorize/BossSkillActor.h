#pragma once
#include "../D2DGameEngine/Actor.h"
#include "../D2DGameEngine/Reflection.h"

class BossSkillActor : public Actor, public IReflection
{
protected:
	enum Elemnet { Fire, Water, Light, Dark };
	enum BossSkillType { Projectile, Range };

public:
	BossSkillActor(class World* _world);
	virtual ~BossSkillActor();



protected:
	bool				isDispel = false;
	bool				isFragile = false;
	BossSkillType		type = Projectile;
	float				dispelTime = 0.f;
	float				skillDuration = 0.f;
	// 스킬 시전 타이밍은 std::max(DispelTime - SkillDuration, 0);
	float				damage = 0.f;
	float				speed = 0.f;
	float				duration = 0.f;
	int					disfellCommandCount = 0;
	std::vector<BYTE>	disfellCommand = {};
	int					dissfellindex = 0;

public:
	virtual void FixedUpdate(float _fixedRate) override;
	virtual void OnClicked() override;

	/**
	 * @brief 디스펠 하는 것.
	 */
	bool Disfell(int _element);

	/**
	 * @brief 갯수 설정 후 사용 해주면 디스펠 command가 자동으로 만들어짐
	 */
	void CreateDisfellCommand();

	/**
	 * @brief 디스펠시 액션과 디스펠 실패시 액션
	 * 삭속 받아서 사용 하길 권장
	 */
	virtual void DisfellAction() {}
	virtual void DisfellFailAction() {}

protected:
	class MovementComponent* mv;
	class BitmapComponent* bm;
	class ClickComponent* clickComponent = nullptr;

	// IReflection을(를) 통해 상속됨
	void ReflectionIn() = 0;
	void ReflectionOut() = 0;
};

