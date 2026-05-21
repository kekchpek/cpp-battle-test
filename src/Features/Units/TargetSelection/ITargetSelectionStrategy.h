#pragma once

#include <Core/Ecs/WorldDomain.h>

namespace sw::core::ecs
{
	class World;
}

namespace sw::core::features
{
	class IMapService;

	class ITargetSelectionStrategy
	{
	public:
		ITargetSelectionStrategy() = default;
		ITargetSelectionStrategy(const ITargetSelectionStrategy&) = delete;
		ITargetSelectionStrategy(ITargetSelectionStrategy&&) = delete;
		ITargetSelectionStrategy& operator=(const ITargetSelectionStrategy&) = delete;
		ITargetSelectionStrategy& operator=(ITargetSelectionStrategy&&) = delete;

		virtual ~ITargetSelectionStrategy() = default;

		[[nodiscard]] virtual ecs::Entity selectTarget(
			ecs::Entity caster,
			ecs::World& world,
			IMapService& mapService) = 0;
	};
}
