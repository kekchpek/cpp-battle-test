#include "RangeBasedTargetSelection.h"

#include <Features/MapManagement/IMapService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/Skills/UnitSkillHelpers.h>
#include <Core/Ecs/World.h>

namespace sw::core::features
{
	RangeBasedTargetSelection::RangeBasedTargetSelection(uint32_t minChebyshevDistance) :
			_minChebyshevDistance(minChebyshevDistance)
	{
	}

	ecs::Entity RangeBasedTargetSelection::selectTarget(
		ecs::Entity caster,
		ecs::World& world,
		IMapService& mapService)
	{
		const auto* unit = world.stash<UnitData>().get(caster);
		if (unit == nullptr || !unit->isAlive())
		{
			return ecs::kInvalidEntity;
		}

		uint32_t casterX = 0;
		uint32_t casterY = 0;
		if (!skills::tryGetEntityLocation(world, caster, casterX, casterY))
		{
			return ecs::kInvalidEntity;
		}

		const uint32_t maxChebyshevDistance = unit->range() + 1;

		const std::vector<ecs::Entity> candidates = skills::collectAliveEnemiesAtChebyshevDistance(
			caster,
			world,
			mapService,
			casterX,
			casterY,
			_minChebyshevDistance,
			maxChebyshevDistance);

		return skills::pickTarget(candidates);
	}
}
