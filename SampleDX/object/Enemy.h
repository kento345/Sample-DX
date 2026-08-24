#pragma once
#include"GameObject.h"
class Enemy : public GameObject
{
public:

	Enemy() = default;

	~Enemy() = default;

	void update() noexcept override;
};

