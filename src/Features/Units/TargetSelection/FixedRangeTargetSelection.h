#pragma once

#include <Features/Units/TargetSelection/ITargetSelectionStrategy.h>

#include <cstdint>

namespace sw::core::features
{
	class FixedRangeTargetSelection final : public ITargetSelectionStrategy
	{
	public:
		FixedRangeTargetSelection(uint32_t minChebyshevDistance, uint32_t maxChebyshevDistance);

		[[nodiscard]] ecs::Entity selectTarget(
			ecs::Entity caster,
			ecs::World& world,
			IMapService& mapService) override;

	private:
		uint32_t _minChebyshevDistance{};
		uint32_t _maxChebyshevDistance{};
	};
}
