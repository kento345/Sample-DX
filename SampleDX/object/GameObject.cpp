#include "GameObject.h"
#include"../draw_resource/ShapeContainer.h"
#include<cmath>

namespace {
	struct ConstBufferData
	{
		DirectX::XMMATRIX world_{}; //ワールド行列
		DirectX::XMFLOAT4 color_{};	//カラー(RGBA)
	};
}


namespace game {

	void GameObject::initialize() noexcept
	{
		createDrawBuffer();
	}

    void GameObject::setDrawCommand(const CommandList& commandList, UINT slot)noexcept{
		Object::setDrawCommand(commandList, slot);
		ShapeContainer::instance().draw(commandList, shapeId_);
	}

	void GameObject::createDrawBuffer() noexcept
	{
		if (!constantBuffer_.create(sizeof(ConstBufferData))) {
			assert(false && "GameObject コンスタントバッファの作成に失敗");
		}
	}

	void GameObject::updateDrawBuffer() noexcept
	{
		Object::updateConstantBuffer(ConstBufferData{ DirectX::XMMatrixTranspose(world_),color_ });
	}

	void GameObject::setHandle(UINT64 handle)noexcept {
		handle_ = handle;
	}

	[[nodiscard]] UINT64 GameObject::handle() const noexcept
	{
		return handle_;
	}

	void GameObject::set(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT4 color, UINT64 shapeId) noexcept
	{
		DirectX::XMMATRIX matScale = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
		DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(rot.x, rot.y, rot.z);
		DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
		world_ = matScale * rotation * translation;

		color_ = color;
		shapeId_ = shapeId;

		radius_ = (scale.x + scale.y + scale.z) / (3.0f * 2.0f);
	}



	DirectX::XMMATRIX GameObject::world() const noexcept
	{
		return world_;
	}

	DirectX::XMFLOAT4 GameObject::color() const noexcept
	{
		return color_;
	}
}