#pragma once
#include "../D2DGameEngine/PlayerController.h"
#include "../D2DGameEngine/Debug.h"
#include "Skill.h"

class GPlayerController : public PlayerController
{
	Math::Vector2 destPos;

	//현재 발동중인 스킬
	class Skill* nowSkill = nullptr;

	//스킬 인스턴스 맵 
	using SkillRegistry = std::unordered_multimap<std::type_index, class Skill*>;
	SkillRegistry skills;

	//FSMPlayer
	class PlayerFSMComponent* playerFSMComponent;

public:
	GPlayerController(class World* _world);
	virtual void SetupInputComponent() override;

	/**
	 * @brief 지정한 스킬을 사용
	 * @tparam T : Skill 상속받은 클래스
	 */
	template <typename T>
	void StartSkill();

	/**
	 * @brief 현재 발동중인 스킬을 끝냅니다. 
	 */
	void EndSkill();

	virtual void BeginPlay() override;
	virtual void Update(float _dt) override;

	void MovePlayer();


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
