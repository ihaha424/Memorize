#include "TestLevel.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"

TestLevel::TestLevel(const std::wstring& name) : Level(name)
{
	Player* player = CreateActor<Player>();

}

TestLevel::~TestLevel()
{
}
