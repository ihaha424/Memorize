#pragma once
#include "IComponent.h"
#include "FSMState.h"

class FSMComponent :
    public IComponent
{
public:
	FSMComponent(){}
	virtual ~FSMComponent();

	template<typename FSMStateType>
	FSMStateType* CreateState(const std::wstring _stateName)
	{
		FSMStateType* pState = new T(pMyPointer, _stateName);
		statesMap.insert(std::make_pair(pState->GetName(), pState));
		return pState;
	}

	virtual void Update(float _dt);

	void SetNextState(std::wstring _stateName);
	const std::wstring GetCurState() const { return currState->GetName(); }
	const std::wstring GetPrevState() const { return prevState->GetName(); }
	FSMState* GetCurStateClass() { return currState; }

protected:
	FSMState* prevState = nullptr;	// 다음 상태
	FSMState* currState = nullptr;  // 현재 상태	
	FSMState* nextState = nullptr;	// 다음 상태

	std::map<std::wstring, FSMState*> statesMap;	// 생성된 FSMState들을 저장하는 컨테이너
};

