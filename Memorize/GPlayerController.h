#pragma once
#include "../D2DGameEngine/PlayerController.h"
#include "../D2DGameEngine/Debug.h"
#include "../D2DGameEngine/IEventListener.h"
#include "DisfellEvent.h"
#include "Skill.h"
#include "Signal.h"
#include "Player.h"

class GPlayerController : public PlayerController, public IEventListener
{
public:
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

	//For Elemental Master
	class ElementalMasterComponent* elementalMasterComponent;

	//DisfellSkill
	class BossSkillActor* targetSkill;

	//Signal for UI
	Signal<int,int>* OnBeginDisfell;
	Signal<int>* OnDoingDisfell;

	//Mana overload skill
	bool bManaOverload = false;
	const float manaOverloadTime = 10.f;
	float manaOverloadTimer = 0.f;

	//Elemental Master
	bool bElementalMaster = false;
	bool bNowAttacking = false;
public:
	GPlayerController(class World* _world);
	virtual ~GPlayerController();
	virtual void SetupInputComponent() override;

	/**
	 * @brief ��ų �ʱ�ȭ �Լ�
	 */
	void InitializeSkill();

	SkilID GetCurSkillInfo() { return curSkillInfo; }

	int GetPlayerCastingIndex();
	bool isPlayerAfterCasting();
	std::wstring GetPlayerState();


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
	template <typename SkillType>
	void StartSkill();

	/**
	 * @brief ���� �ߵ����� ��ų�� �����ϴ�. 
	 */
	void EndSkill();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;


	void SetRandomSkillReady();

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
	bool CheckMemorize();
	void SwapMemorize() { std::swap(curSkillInfo, MemorizeSkill); player->StartMemorizeEffect(); }

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

	void SetDestPos(Math::Vector2 _destPos) { destPos = _destPos; }
	void OnManaDepleted();

	void Fire();
	void Water();
	void Light();
	void Dark(); 
	void Attack();
	void Move();
	void Memorize();
	void Teleport();
	void Cancellation();
	void DisfellEvent(const DisFellEvent* const _event);
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
