#pragma once

#include <Core/Ecs/WorldDomain.h>

namespace sw::core::features
{
	class IMovementService
	{
	public:
		IMovementService() = default;
		IMovementService(const IMovementService&) = delete;
		IMovementService(IMovementService&&) = delete;
		IMovementService& operator=(const IMovementService&) = delete;
		IMovementService& operator=(IMovementService&&) = delete;

		virtual ~IMovementService() = default;

		virtual bool moveToTarget(ecs::Entity entity) = 0;
	};
}
