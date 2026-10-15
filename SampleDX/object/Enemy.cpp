#include "Enemy.h"
#include"../draw_resource/ShapeContainer.h"
#include"../draw_resource/TrianglePolygon.h"

namespace game {
	void Enemy::initialize() noexcept
	{
		GameObject::initialize();

		auto triId = ShapeContainer::instance().create<TrianglePolygon>();
		set({ 0.0f, 0.0f, 30.0f }, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 1.0f }, { 1.0f, 0.0f, 0.0f, 1 }, triId);
	}

	void Enemy::update() noexcept
	{

	}

	void Enemy::OnHit() noexcept
	{
		color_.y *= 0.95f;
		color_.z *= 0.95f;
	}
}

