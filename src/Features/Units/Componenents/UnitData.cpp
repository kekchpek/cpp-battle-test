#include "UnitData.h"

namespace sw::core::features
{
	void UnitData::init(
		uint32_t unitType,
		uint32_t health,
		uint32_t agility,
		uint32_t power,
		uint32_t range)
	{
		_unitType = unitType;
		_health = health;
		_agility = agility;
		_power = power;
		_range = range;
	}

	void UnitData::applyDamage(uint32_t damage)
	{
		if (damage >= _health)
		{
			_health = 0;
			return;
		}

		_health -= damage;
	}
}
