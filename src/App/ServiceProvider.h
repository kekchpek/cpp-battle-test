#pragma once

#include "IServiceProvider.h"
#include "IServiceRegister.h"

#include <typeindex>
#include <unordered_map>

namespace sw::core::app
{
	class ServiceProvider final :
		public IServiceProvider,
		public IServiceRegister
	{
	public:
		using IServiceRegister::registerService;
		using IServiceProvider::get;

		void registerService(std::type_index type, void* service) override;
		void* getService(std::type_index type) const override;

	private:
		std::unordered_map<std::type_index, void*> _services;
	};
}
