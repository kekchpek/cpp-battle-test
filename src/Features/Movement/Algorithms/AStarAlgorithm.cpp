#include "AStarAlgorithm.h"

#include <algorithm>
#include <queue>
#include <unordered_map>
#include <utility>
#include <vector>

namespace sw::core::features
{
	namespace
	{
		using Cell = std::pair<uint32_t, uint32_t>;

		[[nodiscard]] uint64_t cellKey(uint32_t x, uint32_t y)
		{
			return (static_cast<uint64_t>(x) << 32) | static_cast<uint64_t>(y);
		}

		[[nodiscard]] Cell decodeKey(uint64_t key)
		{
			return {
				static_cast<uint32_t>(key >> 32),
				static_cast<uint32_t>(key & 0xFFFFFFFFu)};
		}

		[[nodiscard]] uint32_t chebyshevDistance(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
		{
			const uint32_t dx = x0 > x1 ? x0 - x1 : x1 - x0;
			const uint32_t dy = y0 > y1 ? y0 - y1 : y1 - y0;
			return std::max(dx, dy);
		}

		[[nodiscard]] bool isWalkable(
			uint32_t x,
			uint32_t y,
			const AStarAlgorithm::IsBlockedFn& isBlocked,
			const AStarAlgorithm::IsInsideFn& isInside)
		{
			return isInside(x, y) && !isBlocked(x, y);
		}

		struct OpenNode
		{
			uint32_t x{};
			uint32_t y{};
			uint32_t f{};
		};

		struct OpenNodeCompare
		{
			bool operator()(const OpenNode& lhs, const OpenNode& rhs) const
			{
				return lhs.f > rhs.f;
			}
		};

		[[nodiscard]] std::vector<Cell> reconstructPath(
			const std::unordered_map<uint64_t, uint64_t>& cameFrom,
			uint64_t startKey,
			uint64_t targetKey)
		{
			std::vector<Cell> path;
			uint64_t currentKey = targetKey;

			while (currentKey != startKey)
			{
				const Cell current = decodeKey(currentKey);
				path.push_back(current);

				const auto it = cameFrom.find(currentKey);
				if (it == cameFrom.end())
				{
					return {};
				}

				currentKey = it->second;
			}

			std::reverse(path.begin(), path.end());
			return path;
		}
	}

	AStarAlgorithm::AStarAlgorithm(uint32_t maxIterations) :
			_maxIterations(maxIterations)
	{
	}

	std::optional<std::tuple<uint32_t, uint32_t>> AStarAlgorithm::findNextStep(
		uint32_t startX,
		uint32_t startY,
		uint32_t targetX,
		uint32_t targetY,
		const IsBlockedFn& isBlocked,
		const IsInsideFn& isInside) const
	{
		if (startX == targetX && startY == targetY)
		{
			return std::nullopt;
		}

		if (!isInside(startX, startY) || !isInside(targetX, targetY))
		{
			return std::nullopt;
		}

		if (!isWalkable(targetX, targetY, isBlocked, isInside))
		{
			return std::nullopt;
		}

		const uint64_t startKey = cellKey(startX, startY);
		const uint64_t targetKey = cellKey(targetX, targetY);

		std::priority_queue<OpenNode, std::vector<OpenNode>, OpenNodeCompare> openSet;
		std::unordered_map<uint64_t, uint64_t> cameFrom;
		std::unordered_map<uint64_t, uint32_t> gScore;
		std::unordered_map<uint64_t, bool> closedSet;

		const uint32_t startH = chebyshevDistance(startX, startY, targetX, targetY);
		openSet.push(OpenNode{startX, startY, startH});
		gScore[startKey] = 0;

		static constexpr int kDirections[8][2] = {
			{-1, -1}, {0, -1}, {1, -1},
			{-1, 0},           {1, 0},
			{-1, 1},  {0, 1},  {1, 1},
		};

		uint32_t iterations = 0;
		while (!openSet.empty())
		{
			if (iterations >= _maxIterations)
			{
				return std::nullopt;
			}

			++iterations;

			const OpenNode current = openSet.top();
			openSet.pop();

			const uint64_t currentKey = cellKey(current.x, current.y);
			if (closedSet.contains(currentKey))
			{
				continue;
			}

			if (currentKey == targetKey)
			{
				std::vector<Cell> path = reconstructPath(cameFrom, startKey, targetKey);
				if (path.empty())
				{
					return std::nullopt;
				}

				return std::tuple<uint32_t, uint32_t>{path.front().first, path.front().second};
			}

			closedSet[currentKey] = true;

			const uint32_t currentG = gScore[currentKey];

			for (const auto& direction : kDirections)
			{
				const int nextX = static_cast<int>(current.x) + direction[0];
				const int nextY = static_cast<int>(current.y) + direction[1];

				if (nextX < 0 || nextY < 0)
				{
					continue;
				}

				const uint32_t nx = static_cast<uint32_t>(nextX);
				const uint32_t ny = static_cast<uint32_t>(nextY);

				if (!isWalkable(nx, ny, isBlocked, isInside))
				{
					continue;
				}

				const uint64_t neighborKey = cellKey(nx, ny);
				if (closedSet.contains(neighborKey))
				{
					continue;
				}

				const uint32_t tentativeG = currentG + 1u;

				const auto gIt = gScore.find(neighborKey);
				if (gIt != gScore.end() && tentativeG >= gIt->second)
				{
					continue;
				}

				cameFrom[neighborKey] = currentKey;
				gScore[neighborKey] = tentativeG;

				const uint32_t h = chebyshevDistance(nx, ny, targetX, targetY);
				openSet.push(OpenNode{nx, ny, tentativeG + h});
			}
		}

		return std::nullopt;
	}
}
