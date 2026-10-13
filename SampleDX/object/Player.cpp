#include "Player.h"
#include"../window/Input.h"
#include"../draw_resource/ShapeContainer.h"
#include"../draw_resource/QuadPolygon.h"


namespace game {

	void Player::initialize()noexcept {
		GameObject::initialize();

		auto quadId = ShapeContainer::instance().create<QuadPolygon>();
		set({ -.2f, 0.0f, 0.1f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { .0f, 1, .0f, 1 }, quadId);
	}

	void Player::update() noexcept
	{
		GameObject::update();

		constexpr float moveSpeed = 0.05f;
		
		DirectX::XMFLOAT3 pos{};
		if (Input::instance().getKey('W')) {
			pos.z += moveSpeed;
		}
		if (Input::instance().getKey('S')) {
			pos.z -= moveSpeed;
		}
		if (Input::instance().getKey('A')) {
			pos.x -= moveSpeed;
		}
		if (Input::instance().getKey('D')) {
			pos.x += moveSpeed;
		}

		DirectX::XMVECTOR temp = DirectX::XMVectorSet(pos.x, pos.y, pos.z, 0.0f);
		world_.r[3] = DirectX::XMVectorAdd(world_.r[3], temp);


	}
}


