#pragma once

namespace sw::core::app
{
	class ServiceProvider;

	class IFeature
	{
	public:
		IFeature() = default;
		IFeature(const IFeature&) = delete;
		IFeature(IFeature&&) = delete;
		IFeature& operator=(const IFeature&) = delete;
		IFeature& operator=(IFeature&&) = delete;

		virtual ~IFeature() = default;

		virtual void setup(ServiceProvider& services) = 0;
	};
}
