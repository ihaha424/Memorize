#include "BaseAttackProjectile.h"
#include "D2DGameEngine/AnimationState.h"
#include "D2DGameEngine/Animator.h"
#include "D2DGameEngine/PolygonComponent.h"

BaseAttackProjectile::BaseAttackProjectile(World* _world) : Projectile(_world)
{
	normalState->SetSprite(L"TestResource/Player/Skill/Skill_BasicAttack01.png");
	normalState->SliceSpriteSheet(240, 181, 0, 0, 0, 0);
	normalState->FrameResize(30);
	normalState->SetFrameDurations({ 2.f / 30 });
	anim->Initialize(normalState);
	box->SetPolygon({ {-100, -40}, {100, -40}, {100, 40}, {-100, 40} });
	bIsPassable = false;
	bEnding = false;
	bCollideWithOtherAttack = false;
}

BaseAttackProjectile::~BaseAttackProjectile()
{
}
