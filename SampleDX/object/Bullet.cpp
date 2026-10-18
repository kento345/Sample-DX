#include "Bullet.h"
#include"Enemy.h"

#include"../draw_resource/ShapeContainer.h"
#include"../draw_resource/QuadPolygon.h"

#include"../object/GameObjectManager.h"

namespace game {
	void game::Bullet::initialize() noexcept
	{
		GameObject::initialize();

		auto quadId = ShapeContainer::instance().create<QuadPolygon>();

		DirectX::XMFLOAT3 parentPos{};
		if (auto parent = GameObjectManager::instance().gameObject(parent_)) {
			DirectX::XMStoreFloat3(&parentPos, parent.value()->world().r[3]);
		}
		set(parentPos,{ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f, 0.3f }, quadId);
	}

	void game::Bullet::update() noexcept
	{
		GameObject::update();

		constexpr float	 moveSpeed = 0.3f;
		DirectX::XMFLOAT3 pos{};
		pos.z += moveSpeed;
		world_ = DirectX::XMMatrixMultiply(world_, DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z));

		GameObjectManager::instance().registerHit(handle());
	}
	void Bullet::OnHit() noexcept
	{
		GameObjectManager::instance().registerDelete(handle());
	}
	UINT64 Bullet::hitTargetTypeId() noexcept
	{
		return id::get<Enemy>();
	}
}