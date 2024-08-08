#pragma once

#include "framework.h"

class FSMComponent;

class FSMState
{
public:
	FSMState(FSMComponent* _owner, std::wstring _name)
		:owner(_owner)
		, name(_name)
	{}
	virtual ~FSMState() {}
	std::wstring name;
	FSMComponent* owner;


public:
	void SetOwner(FSMComponent* _owner) { owner = _owner; }

	const std::wstring& GetName() const { return name; }

	virtual void Enter() = 0;	// 상태 진입전 실행할 내용
	virtual void Update(float _dt) = 0;
	virtual void Exit() = 0;	// 상태 탈출때 실행할 내용
};

template<class T>
concept FSMStateType = std::is_base_of<FSMState, T>::value;
