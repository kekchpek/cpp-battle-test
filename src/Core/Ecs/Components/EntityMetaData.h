#pragma once

#include "Core/Ecs/IStashProvider.h"
#include "Core/Ecs/WorldDomain.h"

#include <cstdint>

namespace sw::core::ecs
{
	class EntityMetaData final
	{
	public:
		explicit EntityMetaData(uint32_t id);
		EntityMetaData(const EntityMetaData&) = default;
		EntityMetaData(EntityMetaData&&) = default;
		EntityMetaData& operator=(const EntityMetaData&) = delete;
		EntityMetaData& operator=(EntityMetaData&&) = delete;

		[[nodiscard]] uint32_t id() const { return _id; }

		[[nodiscard]] static uint32_t entityLogId(IStashProvider& stashProvider, Entity entity);

	private:
		const uint32_t _id;
	};
}
