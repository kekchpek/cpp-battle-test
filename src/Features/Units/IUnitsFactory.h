#pragma once

namespace sw::io
{
	struct SpawnHunter;
	struct SpawnSwordsman;
}

namespace sw::core::features
{
	class IUnitsFactory
	{
	public:
		IUnitsFactory() = default;
		IUnitsFactory(const IUnitsFactory&) = delete;
		IUnitsFactory(IUnitsFactory&&) = delete;
		IUnitsFactory& operator=(const IUnitsFactory&) = delete;
		IUnitsFactory& operator=(IUnitsFactory&&) = delete;

		virtual ~IUnitsFactory() = default;

		virtual void spawnSwordsman(const io::SpawnSwordsman& command) = 0;
		virtual void spawnHunter(const io::SpawnHunter& command) = 0;
	};
}
