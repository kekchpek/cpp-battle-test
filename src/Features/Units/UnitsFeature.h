#pragma once

#include <App/IFeature.h>
#include <Features/Units/SimulationCompletion/UnitsSimulationCompletionController.h>
#include <Features/Units/UnitsDataProvider.h>
#include <Features/Units/UnitsFactory.h>

#include <memory>

namespace sw::core::app
{
	class ServiceProvider;
}

namespace sw::core::features
{
	class UnitsFeature final : public app::IFeature
	{
	public:
		UnitsFeature() = default;
		UnitsFeature(const UnitsFeature&) = delete;
		UnitsFeature(UnitsFeature&&) = delete;
		UnitsFeature& operator=(const UnitsFeature&) = delete;
		UnitsFeature& operator=(UnitsFeature&&) = delete;

		~UnitsFeature() override = default;

		void setup(app::ServiceProvider& services) override;

		[[nodiscard]] bool requiresContinuingSimulation() const override;

	private:
		std::unique_ptr<UnitsDataProvider> _unitsDataProvider;
		std::unique_ptr<UnitsFactory> _unitsFactory;
		std::unique_ptr<UnitsSimulationCompletionController> _simulationCompletionController;
	};
}
