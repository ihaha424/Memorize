#pragma once
#include "../D2DGameEngine/PlayerController.h"
#include "../D2DGameEngine/Debug.h"
#include "Skill.h"

class GPlayerController : public PlayerController
{
	Math::Vector2 destPos;

	//���� �ߵ����� ��ų
	class Skill* nowSkill = nullptr;

	//�ߵ��� ��ų ����
	SkilID curSkillInfo = {};
	//MemorizeSkill
	SkilID MemorizeSkill = {};
	
	//��ų �ν��Ͻ� �� 
	using SkillRegistry = std::unordered_map<ULONGLONG, Skill*>;
	SkillRegistry skills;

	//FSMPlayer
	class PlayerFSMComponent* playerFSMComponent;

public:
	GPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	/**
	 * @brief ��ų �ʱ�ȭ �Լ�
	 */
	void InitializeSkill();

	/**
	 * @brief ��ų ���� & �ʿ� �߰�
	 * @tparam SkillType : concept SkillType = std::is_base_of<Skill, T>::value;
	 */
	template<typename SkillType>
	void CreateSkill()
	{
		SkillType* skill = new SkillType(this);
		skills.insert({ skill->GetID(), skill });
	}

	/**
	 * @brief ������ ��ų�� ���
	 * @tparam T : Skill ��ӹ��� Ŭ����
	 */
	template <typename T>
	void StartSkill();

	/**
	 * @brief ���� �ߵ����� ��ų�� �����ϴ�. 
	 */
	void EndSkill();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	/**
	 * @brief Idle or Move���� ������ ó������ �Ӽ�, �ι�°�� Ÿ���� ��������
	 * @param index element or type
	 * @return ��ų ������ ������ ���� Casting���� �Ѿ�� ���� �ܰ�
	 */
	bool AddSkillInfo(int);
	bool CheckSkillInfo();
	void InitializeSkillInfo();

	/**
	 * @brief �޸������ ��ų�� ������ true ������ �־��ְ� false
	 * @return 
	 */
	bool InputMemorize();
	void InitializeMemorize() { MemorizeSkill.element = ESkillElement::SE_END; MemorizeSkill.type = ESkillType::ST_END; }

	/**
	 * @brief �־��� ���� ���� �˸��� ��ų�� ã�� ��
	 * @param  ESkillElement
	 * @param  ESkillType
	 * @return ��ų
	 */
	Skill* FindSkiil(ESkillElement _skillElement, ESkillType _skillType) 
	{
		SkilID key;
		key.element = _skillElement;
		key.type = _skillType;
		auto iter = skills.find(key.id);
		if (iter != skills.end())
			return iter->second;
		return nullptr;
	};
	Skill* FindCurSkiil()
	{
		auto iter = skills.find(curSkillInfo.id);
		if (iter != skills.end())
			return iter->second;
		return nullptr;
	}


	void Fire();
	void Water();
	void Light();
	void Dark(); 
	void Attack();
	void Move();
	void Memorize();
};

template<typename T>
inline void GPlayerController::StartSkill()
{
	if (nowSkill) return;
	auto range = skills.equal_range(std::type_index(typeid(T)));
	if (range.first != skills.end()) {
		nowSkill = range.first->second;
		nowSkill->UseSkill();
	}
}
