#pragma once
#include <memory>
#include <functional>

#include "Box2D/Box2D.h"
enum CollisionFillter {
	BORDER = 0x0001,
	ENEMY = 0x0002,
	PLAYER = 0x0004
};
struct PhysicComponent
{
	PhysicComponent(uint32_t entity, b2World& box2DEngine, const b2BodyDef& bodyDef = b2BodyDef(), const b2FixtureDef& fixtureDef = b2FixtureDef())
	{
		body = { box2DEngine.CreateBody(&bodyDef),[&box2DEngine](b2Body* pBody) {box2DEngine.DestroyBody(pBody); } };
		body->CreateFixture(&fixtureDef);
		body->SetUserEntity(entity);
	}
	PhysicComponent() = default;
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
};