#pragma once

#include "CollisionEnabled.h"
#include "ObjectType.h"
#include "CollisionResponse.h"

enum class CollsionPropertyPreset {
	NoCollision,
	BlockAll,
	OverlapAll,
	DynamicBlockAll,
	DynamicOverlapAll,
	IgnorePawn,
	OverlapPawn,
	Pawn,
	Character,
	Trigger
};

struct CollisionProperty {
	CollisionEnabled::Type collisionEnabled{ CollisionEnabled::NoCollision };
	ECollisionChannel objectType{ ECollisionChannel::WorldStatic };

	CollisionProperty() {
		for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
			collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Ignore;
		}
	}

	CollisionProperty(CollsionPropertyPreset preset) {
		switch (preset)
		{
		case CollsionPropertyPreset::NoCollision:
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Ignore;
			}
			break;
		case CollsionPropertyPreset::BlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			break;
		case CollsionPropertyPreset::OverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Overlap;
			}
			break;
		case CollsionPropertyPreset::DynamicBlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			break;
		case CollsionPropertyPreset::DynamicOverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Overlap;
			}
			break;
		case CollsionPropertyPreset::IgnorePawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Ignore);
			break;
		case CollsionPropertyPreset::OverlapPawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Pawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Pawn;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollsionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Character:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Character;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Block;
			}
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollsionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Trigger:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
				collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Overlap;
			}
			break;
		}
	}

	CollisionResponse GetCollisionResponse(ECollisionChannel channel) {
		return collisionChannelResponseMap[channel];
	}

	void SetCollsionResponse(ECollisionChannel channel, CollisionResponse response) {
		collisionChannelResponseMap[channel] = response;
	}

private:
	std::unordered_map<ECollisionChannel, CollisionResponse> collisionChannelResponseMap{GetNumCollisionChannels()};
};
