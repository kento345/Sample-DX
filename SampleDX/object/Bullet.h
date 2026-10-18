#pragma once
#include"GameObject.h"

namespace game {
	class Bullet : public GameObject
	{
	public:
		
		Bullet() = default;

		using GameObject::GameObject;
		
		virtual ~Bullet() = default;

	public:

		virtual void initialize()noexcept override;

		virtual void update() noexcept override;

	public:

		virtual void OnHit()noexcept override;

		[[nodiscard]] virtual UINT64 hitTargetTypeId()noexcept override;
	};
}
