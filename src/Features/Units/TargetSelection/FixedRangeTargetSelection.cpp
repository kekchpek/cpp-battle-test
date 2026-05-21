#include "FixedRangeTargetSelection.h"

#include <Features/MapManagement/IMapService.h>
#include <Features/Units/Skills/UnitSkillHelpers.h>

namespace sw::core::features
{
	FixedRangeTargetSelection::FixedRangeTargetSelection(
		uint32_t minChebyshevDistance,
		uint32_t maxChebyshevDistance) :
			_minChebyshevDistance(minChebyshevDistance),
			_maxChebyshevDistance(maxChebyshevDistance)
	{
	}

	ecs::Entity FixedRangeTargetSelection::selectTarget(
		ecs::Entity caster,
		ecs::World& world,
		IMapService& mapService)
	{
		uint32_t casterX = 0;
		uint32_t casterY = 0;
		if (!skills::tryGetEntityLocation(world, caster, casterX, casterY))
		{
			return ecs::kInvalidEntity;
		}

		const std::vector<ecs::Entity> candidates = skills::collectAliveEnemiesAtChebyshevDistance(
			caster,
			world,
			mapService,
			casterX,
			casterY,
			_minChebyshevDistance,
			_maxChebyshevDistance);

		return skills::pickTarget(candidates);
	}
}
