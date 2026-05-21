#pragma once

#include <typeindex>

namespace sw::core::app
{
	class IServiceRegister
	{
	public:
		IServiceRegister() = default;
		IServiceRegister(const IServiceRegister&) = delete;
		IServiceRegister(IServiceRegister&&) = delete;
		IServiceRegister& operator=(const IServiceRegister&) = delete;
		IServiceRegister& operator=(IServiceRegister&&) = delete;

		virtual ~IServiceRegister() = default;

		virtual void registerService(std::type_index type, void* service) = 0;

		template <typename T>
		void registerService(T* service)
		{
			registerService(std::type_index{typeid(T)}, static_cast<void*>(service));
		}
	};
}
