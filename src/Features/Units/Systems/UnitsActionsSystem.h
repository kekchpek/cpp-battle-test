#pragma once

#include <Core/Ecs/ISystem.h>
#include <Core/Ecs/IStashProvider.h>
#include <Core/Ecs/WorldDomain.h>

namespace sw::core::features
{
	class IUnitsDataProvider;
	class IMovementService;
	class IUnitsSimulationCompletionController;

	class UnitsActionsSystem final : public ecs::ISystem
	{
	public:
		UnitsActionsSystem(
			IUnitsDataProvider& unitsDataProvider,
			IMovementService& movementService,
			IUnitsSimulationCompletionController& simulationCompletionController);

		void tick(ecs::IStashProvider& stashProvider) override;

	private:
		IUnitsDataProvider& _unitsDataProvider;
		IMovementService& _movementService;
		IUnitsSimulationCompletionController& _simulationCompletionController;
	};
}
