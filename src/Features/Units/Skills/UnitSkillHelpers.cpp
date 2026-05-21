#include "UnitSkillHelpers.h"

#include <Features/MapManagement/Componenents/MapLocation.h>
#include <Features/MapManagement/IMapService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Core/Ecs/Components/EntityMetaData.h>
#include <Core/Ecs/World.h>
#include <IO/Events/UnitAttacked.hpp>
#include <IO/System/EventLog.hpp>

#include <algorithm>
#include <random>

namespace sw::core::features::skills
{
	namespace
	{
		void tryAddAliveEnemy(
			ecs::Entity caster,
			ecs::World& world,
			ecs::Entity candidate,
			std::vector<ecs::Entity>& candidates)
		{
			if (candidate == ecs::kInvalidEntity || candidate == caster)
			{
				return;
			}

			const auto* unit = world.stash<UnitData>().get(candidate);
			if (unit != nullptr && unit->isAlive())
			{
				candidates.push_back(candidate);
			}
		}

		[[nodiscard]] bool isInsideMap(
			uint32_t x,
			uint32_t y,
			uint32_t mapWidth,
			uint32_t mapHeight)
		{
			return x < mapWidth && y < mapHeight;
		}

		std::mt19937& targetRng()
		{
			static std::mt19937 rng{std::random_device{}()};
			return rng;
		}
	}
	bool tryGetEntityLocation(ecs::World& world, ecs::Entity entity, uint32_t& x, uint32_t& y)
	{
		const auto* location = world.stash<MapLocation>().get(entity);
		if (location == nullptr)
		{
			return false;
		}

		x = location->x();
		y = location->y();
		return true;
	}

	uint32_t chebyshevDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
	{
		const uint32_t dx = x0 > x1 ? x0 - x1 : x1 - x0;
		const uint32_t dy = y0 > y1 ? y0 - y1 : y1 - y0;
		return std::max(dx, dy);
	}

	std::vector<ecs::Entity> collectAliveEnemiesAtChebyshevDistance(
		ecs::Entity caster,
		ecs::World& world,
		IMapService& mapService,
		uint32_t casterX,
		uint32_t casterY,
		uint32_t minChebyshevDistance,
		uint32_t maxChebyshevDistance)
	{
		const auto [mapWidth, mapHeight] = mapService.getMapSize();
		std::vector<ecs::Entity> candidates;

		for (int dx = -static_cast<int>(maxChebyshevDistance);
			dx <= static_cast<int>(maxChebyshevDistance);
			++dx)
		{
			for (int dy = -static_cast<int>(maxChebyshevDistance);
				dy <= static_cast<int>(maxChebyshevDistance);
				++dy)
			{
				const uint32_t cellChebyshev = static_cast<uint32_t>(
					std::max(std::abs(dx), std::abs(dy)));
				if (cellChebyshev == 0
					|| cellChebyshev < minChebyshevDistance
					|| cellChebyshev > maxChebyshevDistance)
				{
					continue;
				}

				const int nx = static_cast<int>(casterX) + dx;
				const int ny = static_cast<int>(casterY) + dy;
				if (nx < 0 || ny < 0)
				{
					continue;
				}

				const uint32_t x = static_cast<uint32_t>(nx);
				const uint32_t y = static_cast<uint32_t>(ny);
				if (!isInsideMap(x, y, mapWidth, mapHeight))
				{
					continue;
				}

				tryAddAliveEnemy(caster, world, mapService.getEntityAt(x, y), candidates);
			}
		}

		return candidates;
	}

	ecs::Entity pickTarget(const std::vector<ecs::Entity>& candidates)
	{
		if (candidates.empty())
		{
			return ecs::kInvalidEntity;
		}

		if (candidates.size() == 1)
		{
			return candidates.front();
		}

		std::uniform_int_distribution<std::size_t> distribution(0, candidates.size() - 1);
		return candidates[distribution(targetRng())];
	}

	bool applyDamageToTarget(
		ecs::Entity caster,
		ecs::Entity target,
		uint32_t damage,
		ecs::World& world,
		EventLog& eventLog)
	{
		auto& units = world.stash<UnitData>();
		auto* targetUnit = units.get(target);
		if (targetUnit == nullptr)
		{
			return false;
		}

		targetUnit->applyDamage(damage);
		const uint32_t targetHp = targetUnit->health();

		eventLog.log(io::UnitAttacked{
			ecs::EntityMetaData::entityLogId(world, caster),
			ecs::EntityMetaData::entityLogId(world, target),
			damage,
			targetHp});

		return true;
	}
}
