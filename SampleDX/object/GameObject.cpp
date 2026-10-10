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
		ShapeContainer::instance().draw(commandList, shaderId_);
	}

	void GameObject::createDrawBuffer() noexcept
	{
		if (!constantBuffer_.create(sizeof(ConstBufferData))) {
			assert(false && "GameObject　コンスタントバッファの作成に失敗");
		}
	}

	void GameObject::updateDrawBuffer() noexcept
	{
		Object::updateConstantBuffer(ConstBufferData{ DirectX::XMMatrixTranspose(world_),color_ });
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