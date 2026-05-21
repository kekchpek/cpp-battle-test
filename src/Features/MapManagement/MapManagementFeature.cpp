#include "MapManagementFeature.h"

#include <Features/MapManagement/Componenents/MapLocation.h>
#include <App/ServiceProvider.h>
#include <Core/Ecs/Components/EntityMetaData.h>
#include <Core/Ecs/World.h>
#include <IO/Commands/CreateMap.hpp>
#include <IO/System/EventLog.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/Events/ErrorOccurred.hpp>
#include <IO/Events/MapCreated.hpp>
#include <IO/Events/UnitMoved.hpp>

namespace sw::core::features
{
	void MapManagementFeature::setup(app::ServiceProvider& services)
	{
		_world = services.get<core::ecs::World>();
		_eventLog = services.get<EventLog>();
		services.registerService<ILocationService>(this);
		services.registerService<IMapService>(this);

		auto* parser = services.get<io::CommandParser>();
		if (parser == nullptr)
		{
			return;
		}

		parser->add<io::CreateMap>([this](io::CreateMap command) {
			createMap(command.width, command.height);
		});
	}

	void MapManagementFeature::logError(const std::string& message)
	{
		if (_eventLog == nullptr)
		{
			return;
		}

		_eventLog->log(io::ErrorOccurred{message});
	}

	void MapManagementFeature::createMap(uint32_t width, uint32_t height)
	{
		if (width <= 0 || height <= 0)
		{
			logError("invalid map size");
			return;
		}

		_mapWidth = width;
		_mapHeight = height;
		_mapEntities.assign(static_cast<std::size_t>(width) * height, ecs::kInvalidEntity);

		if (_eventLog != nullptr)
		{
			_eventLog->log(io::MapCreated{width, height});
		}
	}

	std::size_t MapManagementFeature::cellIndex(uint32_t x, uint32_t y) const
	{
		return static_cast<std::size_t>(y) * _mapWidth + x;
	}

	bool MapManagementFeature::isInsideMap(uint32_t x, uint32_t y) const
	{
		return x < _mapWidth && y < _mapHeight;
	}

	bool MapManagementFeature::createLocation(ecs::Entity entity, uint32_t x, uint32_t y)
	{
		if (_world == nullptr)
		{
			return false;
		}

		if (_mapWidth == 0 || _mapHeight == 0)
		{
			logError("map is not created");
			return false;
		}

		if (!isInsideMap(x, y))
		{
			logError("location is outside the map");
			return false;
		}

		auto& locations = _world->stash<MapLocation>();
		if (locations.has(entity))
		{
			logError("entity already has a location");
			return false;
		}

		const std::size_t cell = cellIndex(x, y);
		if (_mapEntities[cell] != ecs::kInvalidEntity)
		{
			logError("cell is already occupied");
			return false;
		}

		auto& location = locations.add(entity);
		location.set(x, y);
		_mapEntities[cell] = entity;
		return true;
	}

	bool MapManagementFeature::setLocation(ecs::Entity entity, uint32_t x, uint32_t y)
	{
		if (_world == nullptr)
		{
			return false;
		}

		if (_mapWidth == 0 || _mapHeight == 0)
		{
			logError("map is not created");
			return false;
		}

		auto& locations = _world->stash<MapLocation>();
		auto* current = locations.get(entity);
		if (current == nullptr)
		{
			logError("entity has no location");
			return false;
		}

		if (!isInsideMap(current->x(), current->y()))
		{
			logError("source location is outside the map");
			return false;
		}

		if (!isInsideMap(x, y))
		{
			logError("destination location is outside the map");
			return false;
		}

		const std::size_t newCell = cellIndex(x, y);
		const ecs::Entity occupant = _mapEntities[newCell];
		if (occupant != ecs::kInvalidEntity)
		{
			logError("cell is already occupied");
			return false;
		}

		const std::size_t currentCell = cellIndex(current->x(), current->y());
		_mapEntities[currentCell] = ecs::kInvalidEntity;
		_mapEntities[newCell] = entity;
		current->set(x, y);

		if (_eventLog != nullptr)
		{
			_eventLog->log(io::UnitMoved{ecs::EntityMetaData::entityLogId(*_world, entity), x, y});
		}

		return true;
	}

	std::tuple<uint32_t, uint32_t> MapManagementFeature::getMapSize()
	{
		return std::make_tuple(_mapWidth, _mapHeight);
	}

	ecs::Entity MapManagementFeature::getEntityAt(uint32_t x, uint32_t y)
	{
		if (!isInsideMap(x, y))
		{
			return ecs::kInvalidEntity;
		}
		return _mapEntities[cellIndex(x, y)];
	}

	void MapManagementFeature::clearEntity(ecs::Entity entity)
	{
		if (_world == nullptr || _mapWidth == 0 || _mapHeight == 0)
		{
			return;
		}

		const auto* location = _world->stash<MapLocation>().get(entity);
		if (location == nullptr)
		{
			return;
		}

		const uint32_t x = location->x();
		const uint32_t y = location->y();
		if (!isInsideMap(x, y))
		{
			return;
		}

		const std::size_t index = cellIndex(x, y);
		if (_mapEntities[index] == entity)
		{
			_mapEntities[index] = ecs::kInvalidEntity;
		}
	}
}
