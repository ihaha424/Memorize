#include "PrimitiveComponent.h"

PrimitiveComponent::PrimitiveComponent()
{
	SetTickProperties(TICK_PRE_UPDATE | TICK_UPDATE | TICK_POST_UPDATE);
}

PrimitiveComponent::~PrimitiveComponent()
{
}
