#include "UnitsSimulationCompletionController.h"

#include <Features/Units/Componenents/UnitData.h>
#include <Core/Ecs/World.h>

namespace sw::core::features
{
	UnitsSimulationCompletionController::UnitsSimulationCompletionController(ecs::World& world) :
			_world(world)
	{
	}

	void UnitsSimulationCompletionController::setNoActionsThisTick(bool noActionsThisTick)
	{
		_noActionsThisTick = noActionsThisTick;
	}

	bool UnitsSimulationCompletionController::shouldContinueSimulation() const
	{
		if (countAliveUnits() <= 1)
		{
			return false;
		}

		if (_noActionsThisTick)
		{
			return false;
		}

		return true;
	}

	uint32_t UnitsSimulationCompletionController::countAliveUnits() const
	{
		uint32_t aliveUnits = 0;
		auto& units = _world.stash<UnitData>();
		units.forEachEntity([&](ecs::Entity entity) {
			const auto* unit = units.get(entity);
			if (unit != nullptr && unit->isAlive())
			{
				++aliveUnits;
			}
		});
		return aliveUnits;
	}
}
