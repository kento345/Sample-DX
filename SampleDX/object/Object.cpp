#include "Object.h"
#include<cmath>

void Object::update() noexcept
{
	move_ += 0.02f;
	world_ = DirectX::XMMatrixTranslation(0.0f, std::sinf(move_) * 1.5f, 0.0f);
	//world_ = DirectX::XMMatrixIdentity();
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
