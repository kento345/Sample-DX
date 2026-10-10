#pragma once

#include"GameObject.h"

namespace game {
	class Enemy : public GameObject
	{
	public:

		Enemy() = default;

		~Enemy() = default;

		void update() noexcept override;
	};
}


