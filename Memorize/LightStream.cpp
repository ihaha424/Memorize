#include "LightStream.h"
#include "LightStreamEffect.h"
#include "D2DGameEngine/World.h"

LightStream::LightStream(Actor* _owner) : RangeSkill(_owner)
{
	lightStreamEffect = GetWorld()->GetCurLevel()->CreateActor<LightStreamEffect>();

}

LightStream::~LightStream()
{
}
