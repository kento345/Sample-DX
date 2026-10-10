#include "ShapeContainer.h"

void ShapeContainer::draw(const CommandList& commandList, UINT64 id) noexcept
{
	auto it = shapes_.find(id);
	if (it == shapes_.end()) {
		return;
	}

	it->second->draw(commandList);
}
