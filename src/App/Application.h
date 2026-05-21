#pragma once

#include <App/ServiceProvider.h>
#include <App/IFeature.h>
#include <istream>
#include <memory>
#include <vector>

namespace sw
{
	class EventLog;
}

namespace sw::core::app
{
	class Application final
	{
	public:
		void run(std::istream& scenario, EventLog& eventLog);

		IServiceProvider& services() { return _services; }
		IServiceRegister& serviceRegister() { return _services; }

	private:
		ServiceProvider _services;
		std::vector<std::unique_ptr<IFeature>> _features;

		void setupFeatures();

		template <typename TFeature>
		void setupFeature()
		{
			_features.push_back(std::make_unique<TFeature>());
			_features.back()->setup(_services);
		}
	};
}
