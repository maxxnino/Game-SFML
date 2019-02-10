#pragma once
#include "Mouse.h"

class World
{
public:
	World()
	{
		box2DEngine = std::make_unique<b2World>(b2Vec2(0.0f, 0.0f));
		
	}
	void Update(float dt, Keyboard& kbd, Mouse& mouse, Graphics& gfx)
	{
		
	}
	void Draw(Graphics& gfx)
	{
		
	}
	
private:
	std::unique_ptr<b2World> box2DEngine;
};