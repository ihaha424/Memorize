#include "FSMComponent.h"

FSMComponent::~FSMComponent()
{
	for (auto& state : statesMap)
	{
		delete state.second;
	}
	statesMap.clear();
}

void FSMComponent::Update(float _dt)
{
	if (currState != nextState)
	{
		if (currState != nullptr)
		{
			currState->Exit();
		}
		prevState = currState;
		currState = nextState;
		currState->Enter();
	}
	else
	{
		currState->Update(_dt);
	}
}

void FSMComponent::SetNextState(std::wstring _stateName)
{
	auto it = statesMap.find(_stateName);
	if (it != statesMap.end())
	{
		nextState = it->second;
	}
}
