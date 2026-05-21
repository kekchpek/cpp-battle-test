#pragma once

#include <cstdint>
#include <limits>

namespace sw::core::ecs
{
	using Entity = uint32_t;

	inline constexpr Entity kInvalidEntity = std::numeric_limits<Entity>::max();
}
