#include "Arena.h"

#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"

Arena::Arena(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	renderLayer = 0;

	BitmapComponent* arena = CreateComponent<BitmapComponent>();
	arena->SetSprite(L"TestResource/BackGround_Map.png");
	rootComponent = arena;

	PolygonComponent* urWall = CreateComponent<PolygonComponent>();
	Math::Vector2 urWallVertex0 = MAP_VERTEX_0 - Math::Vector2{ 0, 2000 };
	Math::Vector2 urWallVertex1 = MAP_VERTEX_1 + Math::Vector2{ 2000, 0 };
	Math::Vector2 urWallVertex2 = MAP_VERTEX_1;
	Math::Vector2 urWallVertex3 = MAP_VERTEX_0;
	urWall->InitPolygon({ urWallVertex0, urWallVertex1, urWallVertex2, urWallVertex3 });
	urWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	urWall->bGenerateOverlapEvent = false;

	PolygonComponent* rWall = CreateComponent<PolygonComponent>();
	Math::Vector2 rWallVertex0 = MAP_VERTEX_1 + Math::Vector2{ 2000, 0 };
	Math::Vector2 rWallVertex1 = MAP_VERTEX_2 + Math::Vector2{ 2000, 0 };
	Math::Vector2 rWallVertex2 = MAP_VERTEX_2;
	Math::Vector2 rWallVertex3 = MAP_VERTEX_1;
	rWall->InitPolygon({ rWallVertex0, rWallVertex1, rWallVertex2, rWallVertex3 });
	rWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	rWall->bGenerateOverlapEvent = false;

	PolygonComponent* lrWall = CreateComponent<PolygonComponent>();
	Math::Vector2 lrWallVertex0 = MAP_VERTEX_2 + Math::Vector2{ 2000, 0 };
	Math::Vector2 lrWallVertex1 = MAP_VERTEX_3 + Math::Vector2{ 0, 2000 };
	Math::Vector2 lrWallVertex2 = MAP_VERTEX_3;
	Math::Vector2 lrWallVertex3 = MAP_VERTEX_2;
	lrWall->InitPolygon({ lrWallVertex0, lrWallVertex1, lrWallVertex2, lrWallVertex3 });
	lrWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	lrWall->bGenerateOverlapEvent = false;

	PolygonComponent* llWall = CreateComponent<PolygonComponent>();
	Math::Vector2 llWallVertex0 = MAP_VERTEX_3;
	Math::Vector2 llWallVertex1 = MAP_VERTEX_3 + Math::Vector2{ 0, 2000 };
	Math::Vector2 llWallVertex2 = MAP_VERTEX_4 - Math::Vector2{ 2000, 0 };
	Math::Vector2 llWallVertex3 = MAP_VERTEX_4;
	llWall->InitPolygon({ llWallVertex0, llWallVertex1, llWallVertex2, llWallVertex3 });
	llWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	llWall->bGenerateOverlapEvent = false;

	PolygonComponent* lWall = CreateComponent<PolygonComponent>();
	Math::Vector2 lWallVertex0 = MAP_VERTEX_5;
	Math::Vector2 lWallVertex1 = MAP_VERTEX_4;
	Math::Vector2 lWallVertex2 = MAP_VERTEX_4 - Math::Vector2{ 2000, 0 };
	Math::Vector2 lWallVertex3 = MAP_VERTEX_5 - Math::Vector2{ 2000, 0 };
	lWall->InitPolygon({ lWallVertex0, lWallVertex1, lWallVertex2, lWallVertex3 });
	lWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	lWall->bGenerateOverlapEvent = false;

	PolygonComponent* ulWall = CreateComponent<PolygonComponent>();
	Math::Vector2 ulWallVertex0 = MAP_VERTEX_0 - Math::Vector2{ 0, 2000 };
	Math::Vector2 ulWallVertex1 = MAP_VERTEX_0;
	Math::Vector2 ulWallVertex2 = MAP_VERTEX_5;
	Math::Vector2 ulWallVertex3 = MAP_VERTEX_5 - Math::Vector2{ 2000, 0 };
	ulWall->InitPolygon({ ulWallVertex0, ulWallVertex1, ulWallVertex2, ulWallVertex3 });
	ulWall->collisionProperty = CollisionProperty(CollisionPropertyPreset::BlockAll);
	ulWall->bGenerateOverlapEvent = false;

}
