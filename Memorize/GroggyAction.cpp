#include "GroggyAction.h"

bool GroggyAction::IsRunning()
{
    return false;
}

void GroggyAction::Run(float dt)
{
	// TODO;
	if (!started) {
		// Set groggy animation true 

		started = true;
	}
	// �������� �� - Then wait for n seconds
}
