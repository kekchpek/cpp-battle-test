#include "RapidShotSkill.h"

#include "UnitSkillHelpers.h"

#include <Features/MapManagement/IMapService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/TargetSelection/RangeBasedTargetSelection.h>
#include <Core/Ecs/World.h>
#include <IO/System/EventLog.hpp>
#include <App/ServiceProvider.h>

namespace sw::core::features::skills
{
	RapidShotSkill::RapidShotSkill(app::ServiceProvider& services) :
			_world(services.get<ecs::World>()),
			_mapService(services.get<IMapService>()),
			_eventLog(services.get<EventLog>()),
			_targetSelection(std::make_unique<RangeBasedTargetSelection>(3))
	{
	}

	bool RapidShotSkill::tryUse(ecs::Entity caster)
	{
		if (_world == nullptr || _mapService == nullptr || _eventLog == nullptr || _targetSelection == nullptr)
		{
			return false;
		}

		const auto* unit = _world->stash<UnitData>().get(caster);
		if (unit == nullptr || !unit->isAlive() || hasAdjacentUnits(caster))
		{
			return false;
		}

		const ecs::Entity target = _targetSelection->selectTarget(caster, *_world, *_mapService);
		if (target == ecs::kInvalidEntity)
		{
			return false;
		}

		return applyDamageToTarget(
			caster,
			target,
			unit->agility(),
			*_world,
			*_eventLog,
			*_mapService);
	}

	bool RapidShotSkill::hasAdjacentUnits(ecs::Entity caster) const
	{
		uint32_t casterX = 0;
		uint32_t casterY = 0;
		if (!tryGetEntityLocation(*_world, caster, casterX, casterY))
		{
			return true;
		}

		static constexpr int kDirections[8][2] = {
			{-1, -1}, {0, -1}, {1, -1},
			{-1, 0},           {1, 0},
			{-1, 1},  {0, 1},  {1, 1},
		};

		for (const auto& direction : kDirections)
		{
			const int nx = static_cast<int>(casterX) + direction[0];
			const int ny = static_cast<int>(casterY) + direction[1];
			if (nx < 0 || ny < 0)
			{
				continue;
			}

			const ecs::Entity neighbor = _mapService->getEntityAt(
				static_cast<uint32_t>(nx),
				static_cast<uint32_t>(ny));
			if (neighbor != ecs::kInvalidEntity && neighbor != caster)
			{
				return true;
			}
		}

		return false;
	}
}
