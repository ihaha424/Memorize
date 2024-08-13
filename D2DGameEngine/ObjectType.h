#pragma once

#include "framework.h"

enum class ECollisionChannel {
	Camera,
	WorldStatic,
	WorldDynamic,
	Pawn,
	Character,
	ProjectTile,
	NUM_COLLISION_CHANNELS
};

constexpr uint GetNumCollisionChannels() {
	return static_cast<uint>(ECollisionChannel::NUM_COLLISION_CHANNELS);
}

constexpr ECollisionChannel GetCollisionChannelByIndex(uint index) {
	if (index >= GetNumCollisionChannels()) return ECollisionChannel::ProjectTile;
	return static_cast<ECollisionChannel>(index);
}
