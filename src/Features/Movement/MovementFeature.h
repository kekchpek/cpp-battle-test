#pragma once

#include "Features/MapManagement/ILocationService.h"
#include "Features/MapManagement/IMapService.h"
#include "Features/Movement/Algorithms/AStarAlgorithm.h"
#include "IO/System/EventLog.hpp"
#include <App/IFeature.h>
#include <Features/Movement/IMovementService.h>

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
	class MovementFeature final :
		public app::IFeature,
		public IMovementService
	{
	public:
		explicit MovementFeature(uint32_t maxPathfindingIterations = 10'000);
		MovementFeature(const MovementFeature&) = delete;
		MovementFeature(MovementFeature&&) = delete;
		MovementFeature& operator=(const MovementFeature&) = delete;
		MovementFeature& operator=(MovementFeature&&) = delete;

		~MovementFeature() override = default;

		void setup(app::ServiceProvider& services) override;

		[[nodiscard]] bool requiresContinuingSimulation() const override { return false; }

		bool moveToTarget(ecs::Entity entity) override;

	private:
		AStarAlgorithm _pathfinder;
		ILocationService* _locationService{nullptr};
		IMapService* _mapService{nullptr};
		EventLog* _eventLog{nullptr};
		core::ecs::World* _world{nullptr};

		void setMarchTarget(ecs::Entity entity, uint32_t targetX, uint32_t targetY);
	};
}
