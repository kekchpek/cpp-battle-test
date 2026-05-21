#include "UnitsDeathCleanupSystem.h"

#include <Features/MapManagement/Componenents/MapLocation.h>
#include <Features/MapManagement/IMapService.h>
#include <Features/Movement/Componenents/MovementData.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Core/Ecs/Components/EntityMetaData.h>
#include <IO/Events/UnitDied.hpp>
#include <IO/System/EventLog.hpp>

#include <vector>

namespace sw::core::features
{
	UnitsDeathCleanupSystem::UnitsDeathCleanupSystem(IMapService& mapService, EventLog& eventLog) :
			_mapService(mapService),
			_eventLog(eventLog)
	{
	}

	void UnitsDeathCleanupSystem::tick(ecs::IStashProvider& stashProvider)
	{
		auto& units = stashProvider.stash<UnitData>();
		std::vector<ecs::Entity> deadEntities;
		units.forEachEntity([&](ecs::Entity entity) {
			const auto* unit = units.get(entity);
			if (unit != nullptr && !unit->isAlive())
			{
				deadEntities.push_back(entity);
			}
		});

		if (deadEntities.empty())
		{
			return;
		}

		auto& locations = stashProvider.stash<MapLocation>();
		auto& movements = stashProvider.stash<MovementData>();

		for (const ecs::Entity entity : deadEntities)
		{
			_eventLog.log(io::UnitDied{ecs::EntityMetaData::entityLogId(stashProvider, entity)});
			_mapService.clearEntity(entity);
			locations.remove(entity);
			movements.remove(entity);
			units.remove(entity);
		}
	}
}
