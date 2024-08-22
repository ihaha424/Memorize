#pragma once
#include "../D2DGameEngine/IComponent.h"
#include "../D2DGameEngine/Reflection.h"
#include "../D2DGameEngine/ReflectionResource.h"
#include "../D2DGameEngine/SoundManager.h"

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
protected:
	class GPlayerController* controller;
	class Player* player;
	SkilID	id;
	std::wstring strId;
	ESkillType type;
	int conditionCount = 0;
	bool bUnlocked = false;
	float damage = 0.f;
	int mana = 0;
	
	bool bCrash;
	std::vector<BYTE>	commandList{};
	std::wstring text;
	std::wstring infoText;
public:
	float skillDuration = 2.f;
	float castingTime = 0.f;

	Skill(Actor* _owner);
	virtual ~Skill();

	void SetPlayer(GPlayerController* _pc);

	/**
	 * @brief 스킬을 사용합니다. 스킬 사용 카운트가 증가합니다. 
	 */
	virtual void UseSkill();
	virtual void Initialize() {};
	const float GetMana() const { return mana; }
	void SetMana(float _mana)  { mana = _mana; }
	const int	GetCommandSize() const { return commandList.size(); }
	const float	GetCastingTime() const { return castingTime; }
	const float	GetConditionCount() const { return conditionCount; }

	void SetID(ESkillType _type, ESkillElement _element) { id.type = _type; id.element = _element; }
	ULONGLONG GetID() { return id.id; };
	bool IsUnlocked() { return bUnlocked; }
	void Unlock() { bUnlocked = true; }

	std::wstring GetInfoText() { return text; }
	std::wstring GetInfoText2() { return infoText; }

	void SetCommandList(std::vector<BYTE>& _commandList) {
		_commandList.resize(commandList.size());
		std::copy(commandList.begin(), commandList.end(), _commandList.begin());
	}
	
	virtual void ReflectionIn() = 0;
	virtual void ReflectionOut() {}

};

template<class T>
concept  SkillType = std::is_base_of<Skill, T>::value;
