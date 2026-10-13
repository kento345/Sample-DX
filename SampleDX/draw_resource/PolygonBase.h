#pragma once

#include"../directx/CommandList.h"

class PolygonBase
{
public:
	PolygonBase() = default;
	
	virtual ~PolygonBase() = default;
	
	virtual bool create() noexcept = 0;

	virtual void draw(const CommandList& commandList)noexcept = 0;
};

