#pragma once
#include "../D2DGameEngine/IComponent.h"
#include "../D2DGameEngine/Reflection.h"

enum ESkillType
{
	ST_PROJECTILE, ST_RANGE, ST_BUFF, ST_SPECIAL, ST_NONE, ST_END
};

enum ESkillElement
{
	SE_FIRE, SE_WATER, SE_LIGHT, SE_DARKNESS, SE_ELEMENT, SE_NONE_ELEMENT, SE_NONE, SE_END
};

union SkilID
{
	struct
	{
		ESkillType type;
		ESkillElement element;
	};
	unsigned long long id;
};


class Skill : public IComponent, IReflection
{
	static int wholeSkillLevel;

protected:
	class GPlayerController* controller;
	class Player* player;
	SkilID	id;
	std::wstring strId;
	ESkillType type;
	int level = 0;
	int count = 0;
	int mana = 0;
	std::vector<BYTE>	commandList{};
	
public:
	Skill(Actor* _owner);
	virtual ~Skill();

	static int GetWholeSkillLevel() { return wholeSkillLevel; }

	void SetPlayer(GPlayerController* _pc);

	/**
	 * @brief 스킬을 사용합니다. 스킬 사용 카운트가 증가합니다. 
	 */
	virtual void UseSkill();
	const float GetMana() const { return 0.0f; }

	void SetID(ESkillType _type, ESkillElement _element) { id.type = _type; id.element = _element; }
	ULONGLONG GetID() { return id.id; };

	void SetCommandList(std::vector<BYTE>& _commandList) {
		_commandList.resize(commandList.size());
		std::copy(commandList.begin(), commandList.end(), _commandList.begin());
	}

	// IReflection을(를) 통해 상속됨
	void ReflectionIn() override {};
	void ReflectionOut() override {};

	//virtual void ReflectionIn() = 0;
	//virtual void ReflectionOut() = 0;

};

template<class T>
concept  SkillType = std::is_base_of<Skill, T>::value;
