#pragma once

#include "Stash.h"

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace sw::core::ecs
{
	class IStashProvider
	{
	private:
		struct IStashHolder
		{
			virtual ~IStashHolder() = default;
		};

		template <typename T>
		struct StashHolder final : IStashHolder
		{
			Stash<T> stash;
		};

		std::unordered_map<std::type_index, std::unique_ptr<IStashHolder>> _stashes;

	public:
		IStashProvider() = default;
		IStashProvider(const IStashProvider&) = delete;
		IStashProvider(IStashProvider&&) = delete;
		IStashProvider& operator=(const IStashProvider&) = delete;
		IStashProvider& operator=(IStashProvider&&) = delete;

		virtual ~IStashProvider() = default;

		template <typename T>
		Stash<T>& stash()
		{
			const std::type_index key{typeid(T)};
			auto it = _stashes.find(key);
			if (it == _stashes.end())
			{
				auto holder = std::make_unique<StashHolder<T>>();
				auto* stashPtr = &holder->stash;
				it = _stashes.emplace(key, std::move(holder)).first;
				return *stashPtr;
			}

			return static_cast<StashHolder<T>*>(it->second.get())->stash;
		}
	};
}
