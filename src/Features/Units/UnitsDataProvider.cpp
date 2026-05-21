#include "UnitsDataProvider.h"

#include <Features/Units/Skills/CrushingBlowSkill.h>
#include <Features/Units/Skills/RapidShotSkill.h>
#include <Features/Units/Skills/ShadowStrikeSkill.h>
#include <Features/Units/UnitTypes.h>
#include <App/ServiceProvider.h>

namespace sw::core::features
{
	UnitsDataProvider::UnitsDataProvider(app::ServiceProvider& services)
	{
		auto& swordsmanSkills = _skillsByType[kUnitTypeSwordsman];
		swordsmanSkills.push_back(std::make_unique<skills::CrushingBlowSkill>(services));

		auto& hunterSkills = _skillsByType[kUnitTypeHunter];
		hunterSkills.push_back(std::make_unique<skills::RapidShotSkill>(services));
		hunterSkills.push_back(std::make_unique<skills::ShadowStrikeSkill>(services));

		for (auto& [unitType, skills] : _skillsByType)
		{
			auto& pointers = _skillPointersByType[unitType];
			pointers.reserve(skills.size());
			for (const auto& skill : skills)
			{
				pointers.push_back(skill.get());
			}
		}
	}

	std::span<ISkill* const> UnitsDataProvider::getSkills(uint32_t unitType) const
	{
		const auto it = _skillPointersByType.find(unitType);
		if (it == _skillPointersByType.end())
		{
			return {};
		}

		return it->second;
	}
}
