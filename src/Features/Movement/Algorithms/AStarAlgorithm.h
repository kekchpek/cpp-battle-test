#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>

namespace sw::core::features
{
	class AStarAlgorithm
	{
	public:
		using IsBlockedFn = std::function<bool(uint32_t, uint32_t)>;
		using IsInsideFn = std::function<bool(uint32_t, uint32_t)>;

		explicit AStarAlgorithm(uint32_t maxIterations);

		// Returns the next cell on a path from start to target (one step), or nullopt if unreachable / already at target.
		[[nodiscard]] std::optional<std::tuple<uint32_t, uint32_t>> findNextStep(
			uint32_t startX,
			uint32_t startY,
			uint32_t targetX,
			uint32_t targetY,
			const IsBlockedFn& isBlocked,
			const IsInsideFn& isInside) const;

	private:
		uint32_t _maxIterations{};
	};
}
