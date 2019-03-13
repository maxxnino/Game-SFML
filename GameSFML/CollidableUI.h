#pragma once
#include <functional>
#include <memory>
#include "Box2D/Box2D.h"
class CollidableUI
{
public:
	//contructor for GUI
	CollidableUI(b2World& box2DEngine, const b2Vec2& worldPos, float width, float height, bool isCircle = false)
	{
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_kinematicBody; //change body type
			bodyDef.fixedRotation = true;
			bodyDef.position.Set(worldPos.x, worldPos.y); //middle, bottom
			body = { box2DEngine.CreateBody(&bodyDef),[&box2DEngine](b2Body* pBody) {box2DEngine.DestroyBody(pBody); } };
		}
		if (isCircle)
		{
			b2CircleShape kinematicCircle;
			kinematicCircle.m_radius = width;
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &kinematicCircle;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
		}
		else
		{
			b2PolygonShape kinematicBox;
			kinematicBox.SetAsBox(std::max(width - 1.5f, 0.1f), std::max(height - 0.7f, 0.1f));
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &kinematicBox;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
		}
	}
	b2Body& getBody()
	{
		return *body;
	}
	const b2Body& getBody() const
	{
		return *body;
	}
	bool GetMouseState() const
	{
		return isMouseIn;
	}
	void MouseContact()
	{
		isMouseIn = true;
	}
	void MouseLeave()
	{
		isMouseIn = false;
	}
private:
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
	bool isMouseIn = false;
};