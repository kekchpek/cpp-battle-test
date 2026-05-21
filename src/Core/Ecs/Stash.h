#pragma once

#include "WorldDomain.h"

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <vector>

namespace sw::core::ecs
{
	// Sparse-set storage for one component type (ECS-style).
	template <typename T>
	class Stash final
	{
	private:
		std::vector<T> _dense;
		std::vector<Entity> _denseEntities;
		std::vector<size_t> _sparse;

		void ensureSparseCapacity(Entity entity)
		{
			if (static_cast<size_t>(entity) >= _sparse.size())
			{
				size_t newSize = static_cast<size_t>(entity) * 2;
				if (newSize == 0)
				{
					newSize = 1;
				}
				_sparse.resize(newSize, kInvalidEntity);
			}
		}

	public:
		Stash() = default;
		Stash(const Stash&) = delete;
		Stash(Stash&&) = default;
		Stash& operator=(const Stash&) = delete;
		Stash& operator=(Stash&&) = default;

		[[nodiscard]] bool has(Entity entity) const
		{
			return entity < _sparse.size() && _sparse[entity] != kInvalidEntity;
		}

		[[nodiscard]] std::size_t size() const
		{
			return _dense.size();
		}

		[[nodiscard]] bool empty() const
		{
			return _dense.empty();
		}

		[[nodiscard]] const T* get(Entity entity) const
		{
			if (!has(entity))
			{
				return nullptr;
			}
			return &_dense[_sparse[entity]];
		}

		[[nodiscard]] T* get(Entity entity)
		{
			if (!has(entity))
			{
				return nullptr;
			}
			return &_dense[_sparse[entity]];
		}

		template <typename... Args>
		T& add(Entity entity, Args&&... args)
		{
			ensureSparseCapacity(entity);

			if (has(entity))
			{
				throw std::runtime_error("Stash: entity already has a component");
			}

			const size_t denseIndex = _dense.size();
			_sparse[entity] = denseIndex;
			_dense.emplace_back(std::forward<Args>(args)...);
			_denseEntities.push_back(entity);
			return _dense.back();
		}

		void remove(Entity entity)
		{
			if (!has(entity))
			{
				return;
			}

			const size_t denseIndex = _sparse[entity];
			const size_t lastDenseIndex = _dense.size() - 1;

			if (denseIndex != lastDenseIndex)
			{
				_dense[denseIndex] = std::move(_dense[lastDenseIndex]);
				_denseEntities[denseIndex] = _denseEntities[lastDenseIndex];
				_sparse[_denseEntities[denseIndex]] = denseIndex;
			}

			_dense.pop_back();
			_denseEntities.pop_back();
			_sparse[entity] = kInvalidEntity;
		}

		template <typename Func>
		void forEachEntity(Func func)
		{
			for (size_t i = 0; i < _denseEntities.size(); ++i)
			{
				func(_denseEntities[i]);
			}
		}

		void clear()
		{
			_dense.clear();
			_denseEntities.clear();
			_sparse.clear();
		}
	};
}
