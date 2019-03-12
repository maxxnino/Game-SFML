#pragma once
#include <memory>
#include <functional>

#include "Locator.h"
enum CollisionFillter {
	BORDER = 0x0001,
	ENEMY = 0x0002,
	PLAYER = 0x0004
};
struct PhysicComponent
{
	PhysicComponent(uint32_t entity, const b2BodyDef& bodyDef = b2BodyDef(), const b2FixtureDef& fixtureDef = b2FixtureDef())
	{
		body = { Locator::Physic::ref().CreateBody(&bodyDef),[](b2Body* pBody) {Locator::Physic::ref().DestroyBody(pBody); } };
		body->CreateFixture(&fixtureDef);
		body->SetUserEntity(entity);
	}
	PhysicComponent() = default;
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
};