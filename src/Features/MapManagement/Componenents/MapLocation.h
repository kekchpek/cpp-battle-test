#pragma once

#include <cstdint>

namespace sw::core::features
{
	class MapManagementFeature;

	// Координаты задаются только через MapManagementFeature.
	class MapLocation final
	{
		friend class MapManagementFeature;

	public:
		MapLocation() = default;
		MapLocation(const MapLocation&) = delete;
		MapLocation(MapLocation&&) = default;
		MapLocation& operator=(const MapLocation&) = delete;
		MapLocation& operator=(MapLocation&&) = default;

		[[nodiscard]] uint32_t x() const { return _x; }
		[[nodiscard]] uint32_t y() const { return _y; }

	private:
		uint32_t _x{};
		uint32_t _y{};

		void set(uint32_t x, uint32_t y);
	};
}
