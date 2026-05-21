#include "World.h"

namespace sw::core::ecs
{
	void World::addSystem(std::unique_ptr<ISystem>&& system)
	{
		_systems.push_back(std::move(system));
	}

	void World::tick()
	{
		for (const auto& system : _systems)
		{
			system->tick(*this);
		}
	}
}
