#include "Application.h"

#include <Features/MapManagement/MapManagementFeature.h>
#include <Features/Movement/MovementFeature.h>
#include <Features/Units/Componenents/UnitData.h>
#include <Features/Units/UnitsFeature.h>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <Core/Ecs/World.h>

namespace sw::core::app
{
	void Application::run(std::istream& scenario, EventLog& eventLog)
	{
		io::CommandParser parser;
		core::ecs::World world;

		serviceRegister().registerService(&parser);
		serviceRegister().registerService(&world);
		serviceRegister().registerService(&eventLog);

		setupFeatures();
		parser.parse(scenario);

		constexpr uint32_t kMaxSimulationTicks = 10'000;
		uint32_t simulationTicks = 0;
		while (simulationTicks < kMaxSimulationTicks)
		{
			uint32_t aliveUnits = 0;
			auto& units = world.stash<features::UnitData>();
			units.forEachEntity([&](ecs::Entity entity) {
				const auto* unit = units.get(entity);
				if (unit != nullptr && unit->isAlive())
				{
					++aliveUnits;
				}
			});

			if (aliveUnits <= 1)
			{
				break;
			}

			eventLog.incrementTick();
			world.tick();
			++simulationTicks;
		}

		_features.clear();
	}

	void Application::setupFeatures()
	{
		setupFeature<features::MapManagementFeature>();
		setupFeature<features::MovementFeature>();
		setupFeature<features::UnitsFeature>();
	}
}
