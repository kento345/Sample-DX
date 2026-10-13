#pragma once
#include"GameObject.h"

namespace game {
	class Player : public GameObject
	{
	public:

		Player() = default;

		virtual ~Player() = default;

	public:

		virtual void initialize()noexcept override;

		virtual void update() noexcept override;

	};
}



