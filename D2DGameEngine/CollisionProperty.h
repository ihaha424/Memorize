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

using CollisionResponseMap = std::unordered_map<ECollisionChannel, CollisionResponse>;

struct CollisionResponseContainer {

	CollisionResponseContainer() {
		for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
			collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Ignore;
		}
	}

	CollisionResponse GetCollisionResponseToChannel(ECollisionChannel channel) {
		return collisionChannelResponseMap[channel];
	}

	void SetCollsionResponseToChannel(ECollisionChannel channel, CollisionResponse response) {
		collisionChannelResponseMap[channel] = response;
	}

	void SetAllChannels(CollisionResponse response) {
		for (auto& [_, ref] : collisionChannelResponseMap) {
			ref = response;
		}
	}

private:
	CollisionResponseMap collisionChannelResponseMap{ GetNumCollisionChannels() };
};

struct CollisionProperty {
	CollisionEnabled::Type collisionEnabled{ CollisionEnabled::NoCollision };
	ECollisionChannel objectType{ ECollisionChannel::WorldStatic };
	CollisionResponseContainer responseContainer;

	CollisionProperty() {}

	CollisionProperty(CollsionPropertyPreset preset) {
		switch (preset)
		{
		case CollsionPropertyPreset::NoCollision:
			break;
		case CollsionPropertyPreset::BlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			break;
		case CollsionPropertyPreset::OverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::DynamicBlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			break;
		case CollsionPropertyPreset::DynamicOverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::IgnorePawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Ignore);
			break;
		case CollsionPropertyPreset::OverlapPawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Pawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Pawn;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollsionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Character:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Character;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollsionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollsionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollsionPropertyPreset::Trigger:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		}
	}

	CollisionResponse GetCollisionResponse(ECollisionChannel channel) const {
		return responseContainer.GetCollisionResponseToChannel(channel);
	}

	void SetCollsionResponse(ECollisionChannel channel, CollisionResponse response) {
		responseContainer.SetCollsionResponseToChannel(channel, response);
	}

};
