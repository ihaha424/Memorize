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

Math::Matrix SceneComponent::GetWorldTransform() const
{
	Math::Matrix localTransform = S * R * T;
	return parent ? localTransform * parent->GetWorldTransform() : localTransform;
}
