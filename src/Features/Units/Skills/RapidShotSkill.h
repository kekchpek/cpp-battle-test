#pragma once

#include <Features/Units/ISkill.h>
#include <Features/Units/TargetSelection/ITargetSelectionStrategy.h>

#include <memory>

namespace sw
{
	class EventLog;
}

namespace sw::core::app
{
	class ServiceProvider;
}

namespace sw::core::ecs
{
	class World;
}

namespace sw::core::features
{
	class IMapService;

	namespace skills
	{
		class RapidShotSkill final : public ISkill
		{
		public:
			explicit RapidShotSkill(app::ServiceProvider& services);

			[[nodiscard]] bool tryUse(ecs::Entity caster) override;

		private:
			[[nodiscard]] bool hasAdjacentUnits(ecs::Entity caster) const;

			ecs::World* _world{};
			IMapService* _mapService{};
			EventLog* _eventLog{};
			std::unique_ptr<ITargetSelectionStrategy> _targetSelection;
		};
	}
}
