#include "UnitsFactory.h"

#include <Features/MapManagement/ILocationService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/UnitTypes.h>
#include <Core/Ecs/Components/EntityMetaData.h>
#include <Core/Ecs/World.h>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/Events/ErrorOccurred.hpp>
#include <IO/Events/UnitSpawned.hpp>
#include <IO/System/EventLog.hpp>

#include <string>

namespace sw::core::features
{
	UnitsFactory::UnitsFactory(
		app::ServiceProvider& /*services*/,
		ecs::World& world,
		ILocationService& locationService,
		EventLog& eventLog) :
			_world(world),
			_locationService(locationService),
			_eventLog(eventLog)
	{
	}

	void UnitsFactory::spawnSwordsman(const io::SpawnSwordsman& command)
	{
		spawnUnit(
			static_cast<ecs::Entity>(command.unitId),
			kUnitTypeSwordsman,
			command.x,
			command.y,
			command.hp,
			0,
			command.strength,
			0);
	}

	void UnitsFactory::spawnHunter(const io::SpawnHunter& command)
	{
		spawnUnit(
			static_cast<ecs::Entity>(command.unitId),
			kUnitTypeHunter,
			command.x,
			command.y,
			command.hp,
			command.agility,
			command.strength,
			command.range);
	}

	void UnitsFactory::spawnUnit(
		ecs::Entity entity,
		uint32_t unitType,
		uint32_t x,
		uint32_t y,
		uint32_t health,
		uint32_t agility,
		uint32_t power,
		uint32_t range)
	{
		auto& metaStash = _world.stash<ecs::EntityMetaData>();
		if (!metaStash.has(entity))
		{
			metaStash.add(entity, static_cast<uint32_t>(entity));
		}

		auto& units = _world.stash<UnitData>();
		if (!units.has(entity))
		{
			units.add(entity).init(unitType, health, agility, power, range);
		}

		if (!_locationService.createLocation(entity, x, y))
		{
			_eventLog.log(io::ErrorOccurred{"failed to create location for unit"});
			return;
		}

		_eventLog.log(io::UnitSpawned{
			ecs::EntityMetaData::entityLogId(static_cast<ecs::IStashProvider&>(_world), entity),
			std::string(unitTypeName(unitType)),
			x,
			y});

	}
}
