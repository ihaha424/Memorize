#include "CameraActor.h"
#include "CameraComponent.h"
#include "World.h"

CameraActor::CameraActor(class World* _world) : Actor(_world)
{
	CameraComponent* camera = CreateComponent<CameraComponent>();
	rootComponent = camera;
	components.insert({ std::type_index(typeid(CameraComponent)), camera });
}

CameraActor::~CameraActor()
{
}
