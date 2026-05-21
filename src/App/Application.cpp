#include "Application.h"

#include <Features/MapManagement/MapManagementFeature.h>
#include <Features/Movement/MovementFeature.h>
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
			eventLog.incrementTick();
			world.tick();
			++simulationTicks;

			bool requiresContinuingSimulation = false;
			for (const auto& feature : _features)
			{
				if (feature->requiresContinuingSimulation())
				{
					requiresContinuingSimulation = true;
					break;
				}
			}

			if (!requiresContinuingSimulation)
			{
				break;
			}
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
