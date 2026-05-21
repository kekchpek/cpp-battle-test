#include "EntityMetaData.h"

namespace sw::core::ecs
{
	EntityMetaData::EntityMetaData(uint32_t id) :
			_id(id)
	{
	}

	uint32_t EntityMetaData::entityLogId(IStashProvider& stashProvider, Entity entity)
	{
		const auto* meta = stashProvider.stash<EntityMetaData>().get(entity);
		if (meta == nullptr)
		{
			return 0;
		}

		return meta->id();
	}
}
