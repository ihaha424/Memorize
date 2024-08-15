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
	// ��ų ���� Ÿ�̹��� std::max(DispelTime - SkillDuration, 0);
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
	 * @brief ���� �ϴ� ��.
	 */
	bool Disfell(int _element);

	/**
	 * @brief ���� ���� �� ��� ���ָ� ���� command�� �ڵ����� �������
	 */
	void CreateDisfellCommand();

	/**
	 * @brief ����� �׼ǰ� ���� ���н� �׼�
	 * ��� �޾Ƽ� ��� �ϱ� ����
	 */
	virtual void DisfellAction() {}
	virtual void DisfellFailAction() {}

protected:
	class MovementComponent* mv;
	class BitmapComponent* bm;
	class ClickComponent* clickComponent = nullptr;

	// IReflection��(��) ���� ��ӵ�
	void ReflectionIn() = 0;
	void ReflectionOut() = 0;
};

