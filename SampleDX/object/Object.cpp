#include "Object.h"

namespace game {
	void Object::setDrawCommand(const CommandList& commandList, UINT slot)noexcept {
		commandList.get()->SetGraphicsRootDescriptorTable(
			slot,
			constantBuffer_.getGpuDescriptorHandle());
	}
}