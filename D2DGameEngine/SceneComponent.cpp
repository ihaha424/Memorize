#include "SceneComponent.h"

void SceneComponent::AddChild(SceneComponent* child)
{
	child->parent = this;
	children.push_back(child);
}

void SceneComponent::RemoveChild(SceneComponent* child)
{
	child->parent = nullptr;
	children.remove(child);
}

D2D_Mat3x2F SceneComponent::GetWorldTransform() const
{
	D2D_Mat3x2F localTransform = S * R * T;
	return parent ? localTransform * parent->GetWorldTransform() : localTransform;
}
