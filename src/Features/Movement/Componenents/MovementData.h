#pragma once

#include <cstdint>

namespace sw::core::features
{
	class MovementFeature;

	class MovementData final
	{
		friend class MovementFeature;

	public:
		MovementData() = default;
		MovementData(const MovementData&) = delete;
		MovementData(MovementData&&) = default;
		MovementData& operator=(const MovementData&) = delete;
		MovementData& operator=(MovementData&&) = default;

		[[nodiscard]] bool hasTarget() const { return _hasTarget; }
		[[nodiscard]] uint32_t targetX() const { return _targetX; }
		[[nodiscard]] uint32_t targetY() const { return _targetY; }

	private:
		bool _hasTarget{false};
		uint32_t _targetX{};
		uint32_t _targetY{};

		void setTarget(uint32_t x, uint32_t y);
		void clearTarget();
	};
}
