#pragma once
#include "../Graphics.h"
class IDrawSystem
{
public:
	virtual void Draw(Graphics& gfx) const = 0;
};