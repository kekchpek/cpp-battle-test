#pragma once

namespace sw::core::features
{
	class IUnitsSimulationCompletionController
	{
	public:
		IUnitsSimulationCompletionController() = default;
		IUnitsSimulationCompletionController(const IUnitsSimulationCompletionController&) = delete;
		IUnitsSimulationCompletionController(IUnitsSimulationCompletionController&&) = delete;
		IUnitsSimulationCompletionController& operator=(const IUnitsSimulationCompletionController&) = delete;
		IUnitsSimulationCompletionController& operator=(IUnitsSimulationCompletionController&&) = delete;

		virtual ~IUnitsSimulationCompletionController() = default;

		virtual void setNoActionsThisTick(bool noActionsThisTick) = 0;

		[[nodiscard]] virtual bool shouldContinueSimulation() const = 0;
	};
}
