#pragma once

#include <memory>

namespace sw::core::ecs
{
	class ISystem;

	class ISystemsRegister
	{
	public:
		virtual ~ISystemsRegister() = default;

		virtual void addSystem(std::unique_ptr<ISystem>&& system) = 0;
	};
}
