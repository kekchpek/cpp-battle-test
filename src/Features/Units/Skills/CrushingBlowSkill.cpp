#include "CrushingBlowSkill.h"

#include "UnitSkillHelpers.h"

#include <Features/MapManagement/IMapService.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/TargetSelection/FixedRangeTargetSelection.h>
#include <Core/Ecs/World.h>
#include <IO/System/EventLog.hpp>
#include <App/ServiceProvider.h>

namespace sw::core::features::skills
{
	CrushingBlowSkill::CrushingBlowSkill(app::ServiceProvider& services) :
			_world(services.get<ecs::World>()),
			_mapService(services.get<IMapService>()),
			_eventLog(services.get<EventLog>()),
			_targetSelection(std::make_unique<FixedRangeTargetSelection>(1, 1))
	{
	}

	bool CrushingBlowSkill::tryUse(ecs::Entity caster)
	{
		if (_world == nullptr || _mapService == nullptr || _eventLog == nullptr || _targetSelection == nullptr)
		{
			return false;
		}

		const auto* unit = _world->stash<UnitData>().get(caster);
		if (unit == nullptr)
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
			unit->power(),
			*_world,
			*_eventLog);
	}
}
