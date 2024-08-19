#include "BrainComponent.h"

#include "AIController.h"

BrainComponent::BrainComponent(Actor* _aiOwner) : IComponent(_aiOwner)
{
	//if (typeid(_aiOwner) != typeid(AIController))
	//	OBJ_ERROR(-1, "AI Owner must be AIController!");
	aiOwner = (AIController*) _aiOwner;
}

class Pawn* BrainComponent::GetPawn()
{
	return aiOwner->GetPawn();
}
