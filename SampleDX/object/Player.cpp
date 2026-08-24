#include "Player.h"
#include"../window/Input.h"



void Player::update() noexcept
{
	const float pos = 0.05f;
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
