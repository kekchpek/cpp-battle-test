#include "UnitsFeature.h"

#include <Features/Units/IUnitsDataProvider.h>
#include <Features/Units/IUnitsFactory.h>
#include <Features/Units/Systems/UnitsActionsSystem.h>
#include <Features/Units/Systems/UnitsDeathCleanupSystem.h>
#include <Features/Units/SimulationCompletion/UnitsSimulationCompletionController.h>
#include <Features/Units/UnitsDataProvider.h>
#include <Features/Units/UnitsFactory.h>
#include <Features/MapManagement/ILocationService.h>
#include <Features/MapManagement/IMapService.h>
#include <Features/Movement/IMovementService.h>
#include <Core/Ecs/World.h>
#include <IO/Commands/SpawnHunter.hpp>
#include <IO/Commands/SpawnSwordsman.hpp>
#include <IO/System/CommandParser.hpp>
#include <IO/System/EventLog.hpp>
#include <App/ServiceProvider.h>

namespace sw::core::features
{
	void UnitsFeature::setup(app::ServiceProvider& services)
	{
		auto* world = services.get<ecs::World>();
		auto* mapService = services.get<IMapService>();
		auto* locationService = services.get<ILocationService>();
		auto* movementService = services.get<IMovementService>();
		auto* eventLog = services.get<EventLog>();
		auto* parser = services.get<io::CommandParser>();
		if (world == nullptr || mapService == nullptr || locationService == nullptr
			|| movementService == nullptr || eventLog == nullptr || parser == nullptr)
		{
			return;
		}

		_simulationCompletionController = std::make_unique<UnitsSimulationCompletionController>(*world);

		_unitsDataProvider = std::make_unique<UnitsDataProvider>(services);
		_unitsFactory = std::make_unique<UnitsFactory>(services, *world, *locationService, *eventLog);

		services.registerService<IUnitsDataProvider>(_unitsDataProvider.get());
		services.registerService<IUnitsFactory>(_unitsFactory.get());

		parser->add<io::SpawnSwordsman>([this](io::SpawnSwordsman command) {
			_unitsFactory->spawnSwordsman(command);
		});
		parser->add<io::SpawnHunter>([this](io::SpawnHunter command) {
			_unitsFactory->spawnHunter(command);
		});

		world->addSystem(std::make_unique<UnitsActionsSystem>(
			*_unitsDataProvider,
			*movementService,
			*_simulationCompletionController));
		world->addSystem(std::make_unique<UnitsDeathCleanupSystem>(*mapService, *eventLog));
	}

	bool UnitsFeature::requiresContinuingSimulation() const
	{
		if (_simulationCompletionController == nullptr)
		{
			return false;
		}

		return _simulationCompletionController->shouldContinueSimulation();
	}

}
