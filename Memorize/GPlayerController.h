#pragma once
#include "../D2DGameEngine/PlayerController.h"
#include "../D2DGameEngine/Debug.h"
#include "Skill.h"

class GPlayerController : public PlayerController
{
	Math::Vector2 destPos;

	//현재 발동중인 스킬
	class Skill* nowSkill = nullptr;

	//발동할 스킬 정보
	SkilID curSkillInfo = {};
	//MemorizeSkill
	SkilID MemorizeSkill = {};
	
	//스킬 인스턴스 맵 
	using SkillRegistry = std::unordered_map<ULONGLONG, Skill*>;
	SkillRegistry skills;

	//FSMPlayer
	class PlayerFSMComponent* playerFSMComponent;

public:
	GPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	/**
	 * @brief 스킬 초기화 함수
	 */
	void InitializeSkill();

	SkilID GetCurSkillInfo() { return curSkillInfo; }

	int GetPlayerCastingIndex();
	bool isPlayerAfterCasting();

	/**
	 * @brief 스킬 생성 & 맵에 추가
	 * @tparam SkillType : concept SkillType = std::is_base_of<Skill, T>::value;
	 */
	template<typename SkillType>
	void CreateSkill()
	{
		SkillType* skill = new SkillType(this);
		skills.insert({ skill->GetID(), skill });
	}

	/**
	 * @brief 지정한 스킬을 사용
	 * @tparam T : Skill 상속받은 클래스
	 */
	template <typename SkillType>
	void StartSkill();

	/**
	 * @brief 현재 발동중인 스킬을 끝냅니다. 
	 */
	void EndSkill();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	/**
	 * @brief Idle or Move에서 누르면 처음에는 속성, 두번째는 타입이 정해진다
	 * @param index element or type
	 * @return 스킬 구성이 끝나면 이제 Casting으로 넘어가기 위한 단계
	 */
	bool AddSkillInfo(int);
	bool CheckSkillInfo();
	void InitializeSkillInfo();

	/**
	 * @brief 메모라이즈 스킬이 있으면 true 없으면 넣어주고 false
	 * @return 
	 */
	bool CheckMemorize();
	void SwapMemorize() { std::swap(curSkillInfo, MemorizeSkill);}
	void InitializeMemorize() { MemorizeSkill.element = ESkillElement::SE_END; MemorizeSkill.type = ESkillType::ST_END; }

	/**
	 * @brief 주어진 값에 따라 알맞은 스킬을 찾는 것
	 * @param  ESkillElement
	 * @param  ESkillType
	 * @return 스킬
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
	void Teleport();
	void Cancellation();
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
