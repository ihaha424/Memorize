#include "TutorialGuide.h"

TutorialGuide::TutorialGuide()
{
	


}

void TutorialGuide::Update(float dt)
{
	if (index == -1)
	{
		Toggle();
	}
	else
	{
		if (index >= (int)toggleActionToggleMap.size())
			return;

		if (toggleActionConditionMap[index](dt))
			Toggle();
	}
}

void TutorialGuide::Toggle()
{
	if (index >= (int)toggleActionToggleMap.size()) 
		return;

	if (0 <= index && index < (int)toggleActionToggleMap.size())
		toggleActionExitMap[index]();

	++index;

	if (index >= (int)toggleActionToggleMap.size()) {
		return;
	}

	toggleActionToggleMap[index]();
}
