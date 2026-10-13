#pragma once

#include"GameObject.h"

namespace game {
	class Enemy : public GameObject
	{
	public:

		Enemy() = default;

		virtual ~Enemy() = default;
	
	public:
		virtual void initialize()noexcept override;

		virtual void update() noexcept override;

	public:

		virtual void OnHit()noexcept override;
	};
}


