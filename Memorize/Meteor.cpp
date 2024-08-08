#include "Meteor.h"
#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/Actor.h"

Meteor::Meteor()
{
	SceneComponent* metorEffectComponent = GetOwner()->CreateComponent<SceneComponent>();
	metorEffectComponent->AddChild(GetOwner()->CreateComponent<BitmapComponent>());
}

Meteor::~Meteor()
{
}
