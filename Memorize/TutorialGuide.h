#pragma once

#include "common.h"

class TutorialGuide
{
	int index{ -1 } ;

	using ToggleActionConditionList = std::vector<std::function<bool(float)>>;
	using ToggleActionToggleList = std::vector<std::function<void()>>;
	using ToggleActionExitList = std::vector<std::function<void()>>;
	ToggleActionConditionList toggleActionConditionMap;
	ToggleActionToggleList toggleActionToggleMap;
	ToggleActionExitList toggleActionExitMap;

public:

	TutorialGuide();

	void Update(float dt);

	void Toggle();

	void AddToggleAction(
		std::function<bool(float)> toggleCondition,
		std::function<void()> toggleAction,
		std::function<void()> exit) {
		toggleActionConditionMap.push_back(toggleCondition);
		toggleActionToggleMap.push_back(toggleAction);
		toggleActionExitMap.push_back(exit);
	}

};
