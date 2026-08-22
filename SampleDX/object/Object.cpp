#include "Object.h"
#include"../window/Input.h"

#include<cmath>

void Object::initialize(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT4 color) noexcept
{
	//ワールド行列の設定
	world_ = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);
	position_ = pos;

	color_ = color;
}

void Object::update() noexcept
{
    float pos = 0.05f;
	//move_ += 0.02f;
	//world_ = DirectX::XMMatrixTranslation(0.0f, std::sinf(move_) * 1.5f, 0.0f);
	//world_ = DirectX::XMMatrixIdentity();
	if (Input::instance().getKey('W')) {
		position_.z += pos ;
	}
	if (Input::instance().getKey('S')) {
		position_.z -= pos;
	}
	if (Input::instance().getKey('A')) {
		position_.x -= pos;
	}
	if (Input::instance().getKey('D')) {
		position_.x += pos;
	}
	world_ = DirectX::XMMatrixTranslation(position_.x,position_.y,position_.z);
	color_ = DirectX::XMFLOAT4(0.1f, 1.0f, 1.0f, 1.0f);
}

DirectX::XMMATRIX Object::world() const noexcept
{
	return world_;
}

DirectX::XMFLOAT4 Object::color() const noexcept
{
	return color_;
}
