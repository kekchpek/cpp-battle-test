#pragma once

#include <Features/Units/IUnitsDataProvider.h>

#include <memory>
#include <unordered_map>
#include <vector>

namespace sw::core::app
{
	class ServiceProvider;
}

namespace sw::core::features
{
	class UnitsDataProvider final : public IUnitsDataProvider
	{
	public:
		explicit UnitsDataProvider(app::ServiceProvider& services);

		[[nodiscard]] std::span<ISkill* const> getSkills(uint32_t unitType) const override;

	private:
		std::unordered_map<uint32_t, std::vector<std::unique_ptr<ISkill>>> _skillsByType;
		std::unordered_map<uint32_t, std::vector<ISkill*>> _skillPointersByType;
	};
}
