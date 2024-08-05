#include "Actor.h"

Actor::Actor()
{
}

Actor::~Actor()
{
	for (auto component : components)
	{
		delete component;
		component = nullptr;
	}
}


bool Actor::Destroy()
{
	if (status == AS_DESTROY)
		return false;
	status = AS_DESTROY;
	return true;
}
//
//void Actor::FixedUpdate(float _fixedRate)
//{
//	for (auto component : components)
//	{
//		component->FixedUpdate(_fixedRate);
//	}
//}
//
//void Actor::PreUpdate(float _dt)
//{
//	for (auto component : components)
//	{
//		component->PreUpdate(_dt);
//	}
//}
//
//void Actor::Update(float _dt)
//{
//	for (auto component : components)
//	{
//		component->Update(_dt);
//	}
//}
//
//void Actor::PostUpdate(float _dt)
//{
//	for (auto component : components)
//	{
//		component->PostUpdate(_dt);
//	}
//}
//
//void Actor::Render(D2DRenderer* _renderer)
//{
//	for (auto component : components)
//	{
//		component->Render(_renderer);
//	}
//}
//
//void Actor::SetLocation(const Math::Vector2 _location)
//{
//	if (rootComponent != nullptr) 
//		rootComponent->SetWorldLocation(_location);
//}
//
//Math::Vector2 Actor::GetLocation() const
//{
//	if (rootComponent != nullptr) 
//		return rootComponent->GetWorldLocation();
//}
//
//void Actor::SetRotation(const Math::Vector2 _rotation)
//{
//	if (rootComponent != nullptr) 
//		rootComponent->SetWorldRotation(_rotation);
//}
//
//Math::Vector2 Actor::GetRotation() const
//{
//	if (rootComponent != nullptr) 
//		return rootComponent->GetWorldRotation();
//}
//
//void Actor::SetScale(const float _scale)
//{
//	if (rootComponent != nullptr) 
//		rootComponent->SetWorldScale(_scale);
//}
//
//Math::Vector2 Actor::GetScale() const
//{
//	if (rootComponent != nullptr) 
//		return rootComponent->GetWorldScale();
//}
//
//void Actor::SetTransform(const D2D_TMat3x2F _transform)
//{
//	if (rootComponent != nullptr)
//		rootComponent->SetWorldTransform(_transform);
//}
//
//D2D_TMat3x2F Actor::GetTrasnform() const
//{
//	if (rootComponent != nullptr)
//		return rootComponent->GetWorldTransform();
//}
