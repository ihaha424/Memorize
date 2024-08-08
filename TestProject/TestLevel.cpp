#include "TestLevel.h"
#include "Player.h"
#include "../D2DGameEngine/ResourceManager.h"
#include "../D2DGameEngine/SpriteResource.h"
#include "TestPlayerController.h"
#include "TestPlayerController.h"
#include "ChasingWaterBall.h"

TestLevel::TestLevel(class World* _world, const std::wstring& _name) : Level(_world, _name)
{
	
}

TestLevel::~TestLevel()
{
}

void TestLevel::Enter()
{
	Player* player = CreateActor<Player>();
	GPlayerController* pc = CreateActor<GPlayerController>();
	player->SetController(pc);

	ChasingWaterBallProjectile* Water = CreateActor<ChasingWaterBallProjectile>();


}
