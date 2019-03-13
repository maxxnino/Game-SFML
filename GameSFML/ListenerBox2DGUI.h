#pragma once
#include "CollidableUI.h"

class ListenerBox2DGUI : public b2ContactListener
{
public:
	void BeginContact(b2Contact* contact) override
	{
		auto pBodyA = contact->GetFixtureA()->GetBody();
		auto pBodyB = contact->GetFixtureB()->GetBody();
		if (pBodyA->GetType() == b2BodyType::b2_kinematicBody)
		{
			static_cast<CollidableUI*>(pBodyA->GetUserData())->MouseContact();
		}
		else if (pBodyB->GetType() == b2BodyType::b2_kinematicBody)
		{
			static_cast<CollidableUI*>(pBodyB->GetUserData())->MouseContact();
		}
	}
	void EndContact(b2Contact* contact)
	{
		auto pBodyA = contact->GetFixtureA()->GetBody();
		auto pBodyB = contact->GetFixtureB()->GetBody();
		if (pBodyA->GetType() == b2BodyType::b2_kinematicBody)
		{
			static_cast<CollidableUI*>(pBodyA->GetUserData())->MouseLeave();
		}
		else if (pBodyB->GetType() == b2BodyType::b2_kinematicBody)
		{
			static_cast<CollidableUI*>(pBodyB->GetUserData())->MouseLeave();
		}
	}
private:
};