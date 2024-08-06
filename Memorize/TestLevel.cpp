#include "TestLevel.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "../D2DGameEngine/PlayerController.h"

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{
	
}

TestLevel::~TestLevel()
{
}

void TestLevel::Enter()
{
	Player* player = CreateActor<Player>();
	PlayerController* pc = CreateActor< PlayerController>();
	player->SetController(pc);
}
