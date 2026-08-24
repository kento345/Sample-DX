#pragma once
#include"GameObject.h"


class Player : public GameObject
{
public:
	
	Player() = default;
	
	~Player() = default;

	 void update() noexcept override;

};

