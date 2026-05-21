#pragma once

#include "IStashProvider.h"
#include "ISystemsRegister.h"
#include "ISystem.h"

#include <memory>
#include <vector>

namespace sw::core::ecs
{
	class World final : public IStashProvider, public ISystemsRegister
	{
	private:
		std::vector<std::unique_ptr<ISystem>> _systems;

	public:
		World() = default;
		World(const World&) = delete;
		World(World&&) = delete;
		World& operator=(const World&) = delete;
		World& operator=(World&&) = delete;

		~World() = default;

		void addSystem(std::unique_ptr<ISystem>&& system) override;
		void tick();
	};
}
