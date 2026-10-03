#include "CommandAllocator.h"
#include<cassert>


CommandAllocator::~CommandAllocator()
{
	if (commandAllocator_) {
		commandAllocator_->Release();
		commandAllocator_ = nullptr;
	}
}

bool CommandAllocator::create(const D3D12_COMMAND_LIST_TYPE type) noexcept
{
	//コマンドリストのタイプを設定
	type_ = type;

	//コマンドアロケータ
	const auto hr = Device::instance().get()->CreateCommandAllocator(type_, IID_PPV_ARGS(&commandAllocator_));
	if (FAILED(hr))
	{
		assert(false && "コマンドアロケータの作成に失敗\0");
		return false;
	}
	return true;
}

void CommandAllocator::reset() noexcept
{
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です\0");
	}
	commandAllocator_->Reset();
}

ID3D12CommandAllocator* CommandAllocator::get() const noexcept
{
	if (!commandAllocator_) {
		assert(false && "コマンドアロケータが未作成です\0");
		return nullptr;
	}
	return commandAllocator_;
}

D3D12_COMMAND_LIST_TYPE CommandAllocator::getType() const noexcept
{
	if (!commandAllocator_) {
		assert(false && "コマンドリストタイプが未設定です\0");
	}
	return type_;
}
