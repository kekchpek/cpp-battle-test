#pragma once

#include <Core/Ecs/WorldDomain.h>

#include <cstdint>

namespace sw::core::features
{
	class ILocationService
	{
	public:
		ILocationService() = default;
		ILocationService(const ILocationService&) = delete;
		ILocationService(ILocationService&&) = delete;
		ILocationService& operator=(const ILocationService&) = delete;
		ILocationService& operator=(ILocationService&&) = delete;

		virtual ~ILocationService() = default;

		[[nodiscard]] virtual bool createLocation(ecs::Entity entity, uint32_t x, uint32_t y) = 0;
		[[nodiscard]] virtual bool setLocation(ecs::Entity entity, uint32_t x, uint32_t y) = 0;
	};
}
