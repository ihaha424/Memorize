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
