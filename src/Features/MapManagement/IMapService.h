#pragma once

#include <cstdint>
#include <tuple>
#include <Core/Ecs/WorldDomain.h>

namespace sw::core::features
{
	class IMapService
	{
	public:
		IMapService() = default;
		IMapService(const IMapService&) = delete;
		IMapService(IMapService&&) = delete;
		IMapService& operator=(const IMapService&) = delete;
		IMapService& operator=(IMapService&&) = delete;

		virtual std::tuple<uint32_t, uint32_t> getMapSize() = 0;
		virtual ecs::Entity getEntityAt(uint32_t x, uint32_t y) = 0;
		virtual void clearEntity(ecs::Entity entity) = 0;

		virtual ~IMapService() = default;
	};
}