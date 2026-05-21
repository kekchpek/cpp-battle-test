#pragma once

#include <Features/Units/IUnitsFactory.h>
#include <Core/Ecs/WorldDomain.h>

#include <cstdint>

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

namespace sw::io
{
	struct SpawnHunter;
	struct SpawnSwordsman;
}

namespace sw::core::features
{
	class ILocationService;

	class UnitsFactory final : public IUnitsFactory
	{
	public:
		UnitsFactory(
			app::ServiceProvider& services,
			ecs::World& world,
			ILocationService& locationService,
			EventLog& eventLog);

		void spawnSwordsman(const io::SpawnSwordsman& command) override;
		void spawnHunter(const io::SpawnHunter& command) override;

	private:
		ecs::World& _world;
		ILocationService& _locationService;
		EventLog& _eventLog;

		void spawnUnit(
			ecs::Entity entity,
			uint32_t unitType,
			uint32_t x,
			uint32_t y,
			uint32_t health,
			uint32_t agility,
			uint32_t power,
			uint32_t range);
	};
}
