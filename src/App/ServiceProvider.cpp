#include "ServiceProvider.h"

namespace sw::core::app
{
	void ServiceProvider::registerService(const std::type_index type, void* service)
	{
		if (service == nullptr)
		{
			_services.erase(type);
			return;
		}

		_services[type] = service;
	}

	void* ServiceProvider::getService(const std::type_index type) const
	{
		const auto it = _services.find(type);
		if (it == _services.end())
		{
			return nullptr;
		}

		return it->second;
	}
}
