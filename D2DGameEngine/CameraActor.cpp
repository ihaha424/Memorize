#include "CameraActor.h"
#include "CameraComponent.h"
#include "World.h"

CameraActor::CameraActor()
{
	CameraComponent* camera = CreateDefaultSubobject<CameraComponent>();
	components.insert({ std::type_index(typeid(CameraComponent)), camera });
}

CameraActor::~CameraActor()
{
}
