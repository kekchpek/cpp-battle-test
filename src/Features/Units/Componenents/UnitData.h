#pragma once

#include <Core/Ecs/WorldDomain.h>

#include <cstdint>

namespace sw::core::features
{
	class UnitsFactory;

	class UnitData final
	{
		friend class UnitsFactory;

	public:
		UnitData() = default;
		UnitData(const UnitData&) = delete;
		UnitData(UnitData&&) = default;
		UnitData& operator=(const UnitData&) = delete;
		UnitData& operator=(UnitData&&) = delete;

		[[nodiscard]] uint32_t unitType() const { return _unitType; }
		[[nodiscard]] uint32_t health() const { return _health; }
		[[nodiscard]] uint32_t agility() const { return _agility; }
		[[nodiscard]] uint32_t power() const { return _power; }
		[[nodiscard]] uint32_t range() const { return _range; }
		[[nodiscard]] bool isAlive() const { return _health > 0; }

		void applyDamage(uint32_t damage);

	private:
		uint32_t _unitType{};
		uint32_t _health{};
		uint32_t _agility{};
		uint32_t _power{};
		uint32_t _range{};

		void init(
			uint32_t unitType,
			uint32_t health,
			uint32_t agility,
			uint32_t power,
			uint32_t range);
	};
}
