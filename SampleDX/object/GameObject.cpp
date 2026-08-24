#include "GameObject.h"
#include<cmath>

void GameObject::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept
{
	//ワールド行列の設定
	world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	position_ = pos;

	color_ = color;
}

void GameObject::update() noexcept
{
    
}

DirectX::XMMATRIX GameObject::world() const noexcept
{
	return world_;
}

DirectX::XMFLOAT4 GameObject::color() const noexcept
{
	return color_;
}
