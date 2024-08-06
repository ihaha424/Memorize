#include "TestLevel.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "TestPlayerController.h"

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{
	
}

TestLevel::~TestLevel()
{
}

void TestLevel::Enter()
{
	Player* player = CreateActor<Player>();
	TestPlayerController* pc = CreateActor<TestPlayerController>();
	player->SetController(pc);
}
