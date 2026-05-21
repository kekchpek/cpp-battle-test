#pragma once

#include <App/IFeature.h>
#include <Features/MapManagement/ILocationService.h>
#include <Features/MapManagement/IMapService.h>

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

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
	class MapManagementFeature final :
		public app::IFeature,
		public ILocationService,
		public IMapService
	{
	public:
		MapManagementFeature() = default;
		MapManagementFeature(const MapManagementFeature&) = delete;
		MapManagementFeature(MapManagementFeature&&) = delete;
		MapManagementFeature& operator=(const MapManagementFeature&) = delete;
		MapManagementFeature& operator=(MapManagementFeature&&) = delete;

		~MapManagementFeature() override = default;

		void setup(app::ServiceProvider& services) override;

		[[nodiscard]] bool requiresContinuingSimulation() const override { return false; }

		[[nodiscard]] bool createLocation(ecs::Entity entity, uint32_t x, uint32_t y) override;
		[[nodiscard]] bool setLocation(ecs::Entity entity, uint32_t x, uint32_t y) override;

		[[nodiscard]] std::tuple<uint32_t, uint32_t> getMapSize() override;
		[[nodiscard]] ecs::Entity getEntityAt(uint32_t x, uint32_t y) override;
		void clearEntity(ecs::Entity entity) override;

	private:
		core::ecs::World* _world{};
		EventLog* _eventLog{nullptr};
		uint32_t _mapWidth{0};
		uint32_t _mapHeight{0};
		std::vector<ecs::Entity> _mapEntities;

		[[nodiscard]] std::size_t cellIndex(uint32_t x, uint32_t y) const;
		[[nodiscard]] bool isInsideMap(uint32_t x, uint32_t y) const;

		void logError(const std::string& message);
		void createMap(uint32_t width, uint32_t height);
	};
}
