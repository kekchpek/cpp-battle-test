#pragma once

#include <Core/Ecs/WorldDomain.h>

namespace sw::core::features
{
	class ISkill
	{
	public:
		ISkill() = default;
		ISkill(const ISkill&) = delete;
		ISkill(ISkill&&) = delete;
		ISkill& operator=(const ISkill&) = delete;
		ISkill& operator=(ISkill&&) = delete;

		virtual ~ISkill() = default;

		[[nodiscard]] virtual bool tryUse(ecs::Entity caster) = 0;
	};
}
