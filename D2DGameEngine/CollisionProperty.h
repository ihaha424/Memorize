#pragma once

#include "CollisionEnabled.h"
#include "ObjectType.h"
#include "CollisionResponse.h"

enum class CollisionPropertyPreset {
	NoCollision,
	BlockAll,
	OverlapAll,
	DynamicBlockAll,
	DynamicOverlapAll,
	IgnorePawn,
	OverlapPawn,
	Pawn,
	Character,
	Trigger,
	Player,
	Enemy,
	PlayerProjectile,
	EnemyProjectile,
	PlayerPattern,
	EnemyPattern
};

using CollisionResponseMap = std::unordered_map<ECollisionChannel, CollisionResponse>;

struct CollisionResponseContainer {

	CollisionResponseContainer() {
		for (uint i = 0; i < GetNumCollisionChannels(); ++i) {
			collisionChannelResponseMap[GetCollisionChannelByIndex(i)] = CollisionResponse::Ignore;
		}
	}

	CollisionResponse GetCollisionResponseToChannel(ECollisionChannel channel) const {
		auto it = collisionChannelResponseMap.find(channel);
		return it->second;
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

	CollisionProperty(CollisionPropertyPreset preset) {
		switch (preset)
		{
		case CollisionPropertyPreset::NoCollision:
			break;
		case CollisionPropertyPreset::BlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			break;
		case CollisionPropertyPreset::OverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::DynamicBlockAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			break;
		case CollisionPropertyPreset::DynamicOverlapAll:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::IgnorePawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollisionResponse(ECollisionChannel::Pawn, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::OverlapPawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollisionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::Pawn:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Pawn;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollisionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::Character:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Character;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollisionResponse(ECollisionChannel::Pawn, CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Character, CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::Trigger:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::WorldDynamic;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			break;
		case CollisionPropertyPreset::Player:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Player;
			responseContainer.SetAllChannels(CollisionResponse::Block);
			SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::Enemy:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::Enemy;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyPattern, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::PlayerProjectile:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::PlayerProjectile;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyPattern, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::EnemyProjectile:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::EnemyProjectile;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyPattern, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::PlayerPattern:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::PlayerPattern;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Player, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyPattern, CollisionResponse::Ignore);
			break;
		case CollisionPropertyPreset::EnemyPattern:
			collisionEnabled = CollisionEnabled::Type::EnableCollision;
			objectType = ECollisionChannel::EnemyPattern;
			responseContainer.SetAllChannels(CollisionResponse::Overlap);
			SetCollisionResponse(ECollisionChannel::Enemy, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::EnemyPattern, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerProjectile, CollisionResponse::Ignore);
			SetCollisionResponse(ECollisionChannel::PlayerPattern, CollisionResponse::Ignore);
			break;
		}
	}

	CollisionResponse GetCollisionResponse(ECollisionChannel channel) const {
		return responseContainer.GetCollisionResponseToChannel(channel);
	}

	void SetCollisionResponse(ECollisionChannel channel, CollisionResponse response) {
		responseContainer.SetCollsionResponseToChannel(channel, response);
	}

};
