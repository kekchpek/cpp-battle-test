#include "MovementData.h"

namespace sw::core::features
{
	void MovementData::setTarget(uint32_t x, uint32_t y)
	{
		_hasTarget = true;
		_targetX = x;
		_targetY = y;
	}

	void MovementData::clearTarget()
	{
		_hasTarget = false;
		_targetX = 0;
		_targetY = 0;
	}
}
