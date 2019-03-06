#pragma once
#include "Box2D/Box2D.h"
#include "SFML/Graphics.hpp"
#include "PhysicComponent.h"
struct DeathTag {};
struct SpawnEnemyInfo
{
	SpawnEnemyInfo(const PhysicComponent& physicCom)
	{
		bodyDef.type = physicCom.body->GetType();
		fixtureDef.shape = physicCom.body->GetFixtureList()->GetShape();
		fixtureDef.filter = physicCom.body->GetFixtureList()->GetFilterData();
		fixtureDef.density = physicCom.body->GetFixtureList()->GetDensity();
		fixtureDef.friction = physicCom.body->GetFixtureList()->GetFriction();
		fixtureDef.restitution = physicCom.body->GetFixtureList()->GetRestitution();
		pos = physicCom.body->GetPosition();
	}
	b2BodyDef bodyDef;
	b2FixtureDef fixtureDef;
	sf::Sprite sprite;
	b2Vec2 pos;
};
