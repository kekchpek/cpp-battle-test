#pragma once

#include <Features/Units/ISkill.h>

#include <cstdint>
#include <span>

namespace sw::core::features
{
	class IUnitsDataProvider
	{
	public:
		IUnitsDataProvider() = default;
		IUnitsDataProvider(const IUnitsDataProvider&) = delete;
		IUnitsDataProvider(IUnitsDataProvider&&) = delete;
		IUnitsDataProvider& operator=(const IUnitsDataProvider&) = delete;
		IUnitsDataProvider& operator=(IUnitsDataProvider&&) = delete;

		virtual ~IUnitsDataProvider() = default;

		[[nodiscard]] virtual std::span<ISkill* const> getSkills(uint32_t unitType) const = 0;
	};
}
