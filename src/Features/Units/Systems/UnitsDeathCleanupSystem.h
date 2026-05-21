#pragma once

#include <Core/Ecs/ISystem.h>
#include <Core/Ecs/IStashProvider.h>

namespace sw
{
	class EventLog;
}

namespace sw::core::features
{
	class IMapService;

	class UnitsDeathCleanupSystem final : public ecs::ISystem
	{
	public:
		UnitsDeathCleanupSystem(IMapService& mapService, EventLog& eventLog);

		void tick(ecs::IStashProvider& stashProvider) override;

	private:
		IMapService& _mapService;
		EventLog& _eventLog;
	};
}
