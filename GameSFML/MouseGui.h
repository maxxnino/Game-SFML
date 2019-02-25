#pragma once
#include "Mouse.h"
#include "Box2D/Box2D.h"
#include <functional>
class MouseGui
{
public:
	MouseGui(b2World& box2DEngine, float radius = 0.1f, bool isCCD = false)
		:
		radius(radius)
	{
		{
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.fixedRotation = true;
			bodyDef.bullet = isCCD;
			bodyDef.position.Set(0.0f, 0.0f);
			body = { box2DEngine.CreateBody(&bodyDef),[&box2DEngine](b2Body* pBody) {box2DEngine.DestroyBody(pBody); } };
		}
		b2CircleShape circleShape;
		circleShape.m_radius = radius;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.isSensor = true;
		body->CreateFixture(&fixtureDef);
	}
	void Draw(Graphics& gfx) const
	{
		gfx.DrawRectDim(ConvertToScreenPos(body->GetPosition(), Camera::scalePixel, Camera::coordinateOffsetX, Camera::coordinateOffsetY),
			Camera::scalePixel, Camera::scalePixel, Colors::Gray);
	}
	void Update(float dt, Mouse& mouse)
	{
		/*body->SetTransform(ConvertToWorldPos(mouse.GetPos(), (float)Camera::scalePixel,
			Camera::coordinateOffsetX, Camera::coordinateOffsetY), 0.0f);*/
		body->SetLinearVelocity((1.0f / dt) * (ConvertToWorldPos(mouse.GetPos(), (float)Camera::scalePixel,
			Camera::coordinateOffsetX, Camera::coordinateOffsetY) - body->GetPosition()));
	}
private:
	std::unique_ptr<b2Body, std::function<void(b2Body*)>> body;
	const float radius;
};