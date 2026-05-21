#include "UnitsActionsSystem.h"

#include <Features/Movement/IMovementService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/IUnitsDataProvider.h>
#include <Features/Units/SimulationCompletion/IUnitsSimulationCompletionController.h>

namespace sw::core::features
{
	UnitsActionsSystem::UnitsActionsSystem(
		IUnitsDataProvider& unitsDataProvider,
		IMovementService& movementService,
		IUnitsSimulationCompletionController& simulationCompletionController) :
			_unitsDataProvider(unitsDataProvider),
			_movementService(movementService),
			_simulationCompletionController(simulationCompletionController)
	{
	}

	void UnitsActionsSystem::tick(ecs::IStashProvider& stashProvider)
	{
		bool anyActionThisTick = false;
		auto& units = stashProvider.stash<UnitData>();
		units.forEachEntity([&](ecs::Entity entity) {
			const auto* unit = units.get(entity);
			if (unit == nullptr)
			{
				return;
			}

			for (ISkill* const skill : _unitsDataProvider.getSkills(unit->unitType()))
			{
				if (skill != nullptr && skill->tryUse(entity))
				{
					anyActionThisTick = true;
					return;
				}
			}

			if (_movementService.moveToTarget(entity))
			{
				anyActionThisTick = true;
			}
		});

		_simulationCompletionController.setNoActionsThisTick(!anyActionThisTick);
	}
}
