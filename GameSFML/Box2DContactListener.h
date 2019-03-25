#pragma once
#include "Component/CollisionRespondComponent.h"
#include "entt/entt.hpp"
class Box2DContactListener final : public b2ContactListener
{
public:
	Box2DContactListener(entt::DefaultRegistry& ECS) : ECS(ECS) {}
	void BeginContact(b2Contact* contact) final
	{
		auto* ptrBody1 = contact->GetFixtureA()->GetBody();
		auto* ptrBody2 = contact->GetFixtureB()->GetBody();
		if (ptrBody1->GetType() == b2BodyType::b2_staticBody || ptrBody2->GetType() == b2BodyType::b2_staticBody) return;

		auto categoryBits1 = contact->GetFixtureA()->GetFilterData().categoryBits;
		auto entity1 = ptrBody1->GetUserEntity();
		auto categoryBits2 = contact->GetFixtureB()->GetFilterData().categoryBits;
		auto entity2 = ptrBody2->GetUserEntity();
		if (ECS.has<CollisionRespondComponent>(entity1))
		{
			if (ECS.has<CollisionCallbackData>(entity1))
			{
				ECS.get<CollisionCallbackData>(entity1).others.emplace_back(std::make_pair(entity2, categoryBits2));
			}
			else
			{
				ECS.assign<CollisionCallbackData>(entity1).others.emplace_back(std::make_pair(entity2, categoryBits2));
			}
		}
		if (ECS.has<CollisionRespondComponent>(entity2))
		{
			if (ECS.has<CollisionCallbackData>(entity2))
			{
				ECS.get<CollisionCallbackData>(entity2).others.emplace_back(std::make_pair(entity1, categoryBits1));
			}
			else
			{
				ECS.assign<CollisionCallbackData>(entity2).others.emplace_back(std::make_pair(entity1, categoryBits1));
			}
		}
	}
private:
	entt::DefaultRegistry& ECS;
};
