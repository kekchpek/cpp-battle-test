#pragma once

#include <Features/Units/TargetSelection/ITargetSelectionStrategy.h>

#include <cstdint>

namespace sw::core::features
{
	class RangeBasedTargetSelection final : public ITargetSelectionStrategy
	{
	public:
		explicit RangeBasedTargetSelection(uint32_t minChebyshevDistance);

		[[nodiscard]] ecs::Entity selectTarget(
			ecs::Entity caster,
			ecs::World& world,
			IMapService& mapService) override;

	private:
		uint32_t _minChebyshevDistance{};
	};
}
