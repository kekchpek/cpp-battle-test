#pragma once

#include <Core/Ecs/WorldDomain.h>

#include <cstdint>
#include <vector>

namespace sw
{
	class EventLog;
}

namespace sw::core::ecs
{
	class World;
}

namespace sw::core::features
{
	class IMapService;

	namespace skills
	{
		[[nodiscard]] uint32_t chebyshevDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1);

		[[nodiscard]] bool tryGetEntityLocation(
			ecs::World& world,
			ecs::Entity entity,
			uint32_t& x,
			uint32_t& y);

		[[nodiscard]] std::vector<ecs::Entity> collectAliveEnemiesAtChebyshevDistance(
			ecs::Entity caster,
			ecs::World& world,
			IMapService& mapService,
			uint32_t casterX,
			uint32_t casterY,
			uint32_t minChebyshevDistance,
			uint32_t maxChebyshevDistance);

		[[nodiscard]] ecs::Entity pickTarget(const std::vector<ecs::Entity>& candidates);

		bool applyDamageToTarget(
			ecs::Entity caster,
			ecs::Entity target,
			uint32_t damage,
			ecs::World& world,
			EventLog& eventLog,
			IMapService& mapService);
	}
}
