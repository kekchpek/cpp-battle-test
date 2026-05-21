#pragma once

#include <typeindex>

namespace sw::core::app
{
	class IServiceProvider
	{
	public:
		IServiceProvider() = default;
		IServiceProvider(const IServiceProvider&) = delete;
		IServiceProvider(IServiceProvider&&) = delete;
		IServiceProvider& operator=(const IServiceProvider&) = delete;
		IServiceProvider& operator=(IServiceProvider&&) = delete;

		virtual ~IServiceProvider() = default;

		virtual void* getService(std::type_index type) const = 0;

		template <typename T>
		[[nodiscard]] T* get() const
		{
			return static_cast<T*>(getService(std::type_index{typeid(T)}));
		}
	};
}
