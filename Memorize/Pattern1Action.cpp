#include "Pattern1Action.h"

#include "D2DGameEngine/BehaviorTree.h"
#include "D2DGameEngine/World.h"

#include "Player.h"
#include "Boss.h"
#include "Bat.h"

constexpr std::array<Math::Vector2, 8> ANGLES{
	Math::Vector2{0, -1},
	Math::Vector2{SQRT2_F, -SQRT2_F},
	Math::Vector2{1, 0},
	Math::Vector2{-SQRT2_F, -SQRT2_F},
	Math::Vector2{0, 1},
	Math::Vector2{-SQRT2_F, SQRT2_F},
	Math::Vector2{-1, 0},
	Math::Vector2{SQRT2_F, -SQRT2_F},
};
constexpr std::size_t NUM_ANGLES = ANGLES.size();

void Pattern1Action::Run(float dt)
{
	Super::Run(dt);
	// TODO;
	// 
	Math::Vector2 playerLocation = bt->GetKey<Player*>("Player")->GetLocation();
	Math::Vector2 bossLocation = bt->GetKey<Boss*>("Boss")->GetLocation();
	Math::Vector2 toPlayer = (playerLocation - bossLocation);
	toPlayer.Normalize();

	Bat* bat1 = bt->GetWorld()->GetCurLevel()->CreateActor<Bat>();
	Bat* bat2 = bt->GetWorld()->GetCurLevel()->CreateActor<Bat>();
	Bat* bat3 = bt->GetWorld()->GetCurLevel()->CreateActor<Bat>();
	bat1->SetLocation(bossLocation.x, bossLocation.y);
	bat2->SetLocation(bossLocation.x, bossLocation.y);
	bat3->SetLocation(bossLocation.x, bossLocation.y);

	// 플레이어와 가장 작은 각을 만드는 벡터를 찾음
	uint index = 0;
	float smallestAngle = FLOAT_MAX;
	for (uint i = 0; i < NUM_ANGLES; ++i)
	{
		float angle = toPlayer.Dot(ANGLES[i]);
		if (smallestAngle > angle)
		{
			index = i;
			smallestAngle = angle;
		}
	}
	
	Math::Vector2 shotDirection1 = ANGLES[(index - 1) % NUM_ANGLES];
	Math::Vector2 shotDirection2 = ANGLES[index];
	Math::Vector2 shotDirection3 = ANGLES[(index + 1) % NUM_ANGLES];

	bat1->AddVelocity(shotDirection1 * bat1->speed);
	bat2->AddVelocity(shotDirection2 * bat1->speed);
	bat3->AddVelocity(shotDirection3 * bat1->speed);
}
