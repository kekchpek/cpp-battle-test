#pragma once

#include <Features/Units/SimulationCompletion/IUnitsSimulationCompletionController.h>

#include <cstdint>

namespace sw::core::ecs
{
	class World;
}

namespace sw::core::features
{
	class UnitsSimulationCompletionController final : public IUnitsSimulationCompletionController
	{
	public:
		explicit UnitsSimulationCompletionController(ecs::World& world);

		void setNoActionsThisTick(bool noActionsThisTick) override;

		[[nodiscard]] bool shouldContinueSimulation() const override;

	private:
		ecs::World& _world;
		bool _noActionsThisTick{false};

		[[nodiscard]] uint32_t countAliveUnits() const;
	};
}
