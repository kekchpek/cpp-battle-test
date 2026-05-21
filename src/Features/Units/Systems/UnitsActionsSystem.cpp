#include "UnitsActionsSystem.h"

#include <Features/Movement/IMovementService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/IUnitsDataProvider.h>
namespace sw::core::features
{
	UnitsActionsSystem::UnitsActionsSystem(
		IUnitsDataProvider& unitsDataProvider,
		IMovementService& movementService) :
			_unitsDataProvider(unitsDataProvider),
			_movementService(movementService)
	{
	}

	void UnitsActionsSystem::tick(ecs::IStashProvider& stashProvider)
	{
		auto& units = stashProvider.stash<UnitData>();
		units.forEachEntity([&](ecs::Entity entity) {
			const auto* unit = units.get(entity);
			if (unit == nullptr || !unit->isAlive())
			{
				return;
			}

			for (ISkill* const skill : _unitsDataProvider.getSkills(unit->unitType()))
			{
				if (skill != nullptr && skill->tryUse(entity))
				{
					return;
				}
			}

			_movementService.moveToTarget(entity);
		});
	}
}
