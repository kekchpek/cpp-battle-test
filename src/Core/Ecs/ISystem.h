#pragma once

namespace sw::core::ecs
{
	class IStashProvider;

	class ISystem
	{
	public:
		virtual ~ISystem() = default;

		virtual void tick(IStashProvider& stashProvider) = 0;
	};
}
