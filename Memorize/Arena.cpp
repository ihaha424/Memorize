#include "Arena.h"

#include "D2DGameEngine/BitmapComponent.h"
#include "D2DGameEngine/PolygonComponent.h"


Arena::Arena(World* _world) : Actor(_world)
{
	SetTickProperties(TICK_PHYSICS | TICK_UPDATE | TICK_RENDER);

	renderLayer = 0;

	arena = CreateComponent<BitmapComponent>();
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

void Arena::Update(float _dt)
{
	if (earthquake)
	{
		turbulance += _dt * 500.f;
		shockwaveScale += _dt;
		earthquakeTimer -= _dt;
		if (earthquakeTimer <= 0.f)
		{
			earthquakeTimer = 3.f;
			shockwaveScale = 0.f;
			turbulance = 500.f;
			earthquake = false;
		}
	}
}

void Arena::Render(D2DRenderer* _renderer)
{
	if (earthquake)
	{
		_renderer->PushTransform(arena->GetWorldTransform());

		float halfWidth = arena->GetSpriteWidth() / 2.f;
		float halfHeight = arena->GetSpriteHeight() / 2.f;

#ifndef NDEBUG
		D2D_RectF dest{
			.left = -halfWidth,
			.top = -halfHeight,
			.right = halfWidth,
			.bottom = halfHeight
		};

		_renderer->DrawBorder(
			{ dest.left, dest.top },
			{ dest.right, dest.bottom },
			D2D_Color::Green
		);
#endif

		ID2D1DeviceContext* dc = _renderer->GetDC();
		ComPtr<ID2D1Effect> displacementEffect;
		dc->CreateEffect(CLSID_D2D1DisplacementMap, &displacementEffect);
		displacementEffect->SetInput(0, arena->GetSprite()->GetResource());
		displacementEffect->SetValue(D2D1_DISPLACEMENTMAP_PROP_SCALE, 10.f);

		ComPtr<ID2D1Effect> turbulenceEffect;
		dc->CreateEffect(CLSID_D2D1Turbulence, &turbulenceEffect);
		turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_SIZE, D2D1_VECTOR_2F{ arena->GetSpriteWidth(), arena->GetSpriteHeight() });
		turbulenceEffect->SetValue(
			D2D1_TURBULENCE_PROP_BASE_FREQUENCY, 
			D2D1_VECTOR_2F{ 
				1.f / ((uint)turbulance + 1U), 
				1.f / ((uint)turbulance + 1U) 
			}
		);
		//turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_SEED, (uint)turbulance);
		//turbulenceEffect->SetValue(D2D1_TURBULENCE_PROP_NUM_OCTAVES, );
		displacementEffect->SetInputEffect(1, turbulenceEffect.Get());

		dc->DrawImage(displacementEffect.Get(), D2D1_POINT_2F{ -halfWidth , -halfHeight });

		_renderer->PopTransform();
	}
	else
	{
		arena->Render(_renderer);
	}

}
