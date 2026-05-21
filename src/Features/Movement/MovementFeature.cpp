#include "MovementFeature.h"

#include <Features/MapManagement/Componenents/MapLocation.h>
#include <Features/Movement/Algorithms/AStarAlgorithm.h>
#include <Features/Movement/Componenents/MovementData.h>
#include <App/ServiceProvider.h>
#include <Core/Ecs/Components/EntityMetaData.h>
#include <Core/Ecs/World.h>
#include <Core/Ecs/WorldDomain.h>
#include <IO/Commands/March.hpp>
#include <IO/Events/MarchEnded.hpp>
#include <IO/Events/MarchStarted.hpp>
#include <IO/System/CommandParser.hpp>

#include <optional>
#include <utility>

namespace sw::core::features
{
	namespace
	{
		[[nodiscard]] std::optional<std::pair<uint32_t, uint32_t>> findGreedyStepTowardsTarget(
			uint32_t startX,
			uint32_t startY,
			uint32_t targetX,
			uint32_t targetY,
			const AStarAlgorithm::IsBlockedFn& isBlocked,
			const AStarAlgorithm::IsInsideFn& isInside)
		{
			int dx = 0;
			int dy = 0;
			if (targetX > startX)
			{
				dx = 1;
			}
			else if (targetX < startX)
			{
				dx = -1;
			}

			if (targetY > startY)
			{
				dy = 1;
			}
			else if (targetY < startY)
			{
				dy = -1;
			}

			if (dx == 0 && dy == 0)
			{
				return std::nullopt;
			}

			const int nextX = static_cast<int>(startX) + dx;
			const int nextY = static_cast<int>(startY) + dy;
			if (nextX < 0 || nextY < 0)
			{
				return std::nullopt;
			}

			const uint32_t x = static_cast<uint32_t>(nextX);
			const uint32_t y = static_cast<uint32_t>(nextY);
			if (!isInside(x, y) || isBlocked(x, y))
			{
				return std::nullopt;
			}

			return std::pair<uint32_t, uint32_t>{x, y};
		}
	}

	MovementFeature::MovementFeature(uint32_t maxPathfindingIterations) :
			_pathfinder(maxPathfindingIterations)
	{
	}

	void MovementFeature::setup(app::ServiceProvider& services)
	{
		_world = services.get<core::ecs::World>();
		_locationService = services.get<ILocationService>();
		_mapService = services.get<IMapService>();
		_eventLog = services.get<EventLog>();
		services.registerService<IMovementService>(this);

		auto* parser = services.get<io::CommandParser>();
		if (parser == nullptr)
		{
			return;
		}

		parser->add<io::March>([this](io::March command) {
			setMarchTarget(
				static_cast<ecs::Entity>(command.unitId),
				command.targetX,
				command.targetY);
		});
	}

	void MovementFeature::moveToTarget(ecs::Entity entity)
	{
		if (_world == nullptr || _locationService == nullptr || _mapService == nullptr)
		{
			return;
		}

		auto& movements = _world->stash<MovementData>();
		auto* movement = movements.get(entity);
		if (movement == nullptr || !movement->hasTarget())
		{
			return;
		}

		auto& locations = _world->stash<MapLocation>();
		auto* location = locations.get(entity);
		if (location == nullptr)
		{
			return;
		}

		const uint32_t startX = location->x();
		const uint32_t startY = location->y();
		const uint32_t targetX = movement->targetX();
		const uint32_t targetY = movement->targetY();

		if (startX == targetX && startY == targetY)
		{
			movement->clearTarget();
			if (_eventLog != nullptr)
			{
				_eventLog->log(io::MarchEnded{ecs::EntityMetaData::entityLogId(*_world, entity), startX, startY});
			}
			return;
		}

		const auto [mapWidth, mapHeight] = _mapService->getMapSize();

		const auto isInside = [mapWidth, mapHeight](uint32_t x, uint32_t y) {
			return x < mapWidth && y < mapHeight;
		};

		const auto isBlocked = [this, entity](uint32_t x, uint32_t y) {
			const ecs::Entity occupant = _mapService->getEntityAt(x, y);
			return occupant != ecs::kInvalidEntity && occupant != entity;
		};

		// Пытаемся найти следующий шаг с помощью pathfinding алгоритма.
		std::optional<std::pair<uint32_t, uint32_t>> nextStep;
		if (const auto astarStep = _pathfinder.findNextStep(
				startX,
				startY,
				targetX,
				targetY,
				isBlocked,
				isInside);
			astarStep.has_value())
		{
			const auto [x, y] = *astarStep;
			nextStep = std::pair<uint32_t, uint32_t>{x, y};
		}
		// Иначе просто в тупую идем по направлению к цели.
		else
		{
			nextStep = findGreedyStepTowardsTarget(
				startX,
				startY,
				targetX,
				targetY,
				isBlocked,
				isInside);
		}

		if (!nextStep.has_value())
		{
			return;
		}

		const auto [nextX, nextY] = *nextStep;
		if (!_locationService->setLocation(entity, nextX, nextY))
		{
			return;
		}

		if (nextX == targetX && nextY == targetY)
		{
			movement->clearTarget();
			if (_eventLog != nullptr)
			{
				_eventLog->log(io::MarchEnded{ecs::EntityMetaData::entityLogId(*_world, entity), nextX, nextY});
			}
		}
	}

	void MovementFeature::setMarchTarget(ecs::Entity entity, uint32_t targetX, uint32_t targetY)
	{
		if (_world == nullptr)
		{
			return;
		}

		auto& movements = _world->stash<MovementData>();
		if (!movements.has(entity))
		{
			movements.add(entity).setTarget(targetX, targetY);
		}
		else
		{
			movements.get(entity)->setTarget(targetX, targetY);
		}

		if (_eventLog == nullptr)
		{
			return;
		}

		auto& locations = _world->stash<MapLocation>();
		auto* location = locations.get(entity);
		if (location == nullptr)
		{
			return;
		}

		_eventLog->log(io::MarchStarted{
			ecs::EntityMetaData::entityLogId(*_world, entity),
			location->x(),
			location->y(),
			targetX,
			targetY});
	}
}
