#pragma once

#include <cstdint>
#include <string_view>

namespace sw::core::features
{
	inline constexpr uint32_t kUnitTypeSwordsman = 0;
	inline constexpr uint32_t kUnitTypeHunter = 1;

	[[nodiscard]] constexpr std::string_view unitTypeName(uint32_t unitType)
	{
		switch (unitType)
		{
		case kUnitTypeSwordsman:
			return "Swordsman";
		case kUnitTypeHunter:
			return "Hunter";
		default:
			return "Unknown";
		}
	}
}
