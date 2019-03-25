#pragma once
#include "Component/CollisionCallbackComponent.h"
#include"entt/entt.hpp"
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

		if (ECS.has<CollisionCallbackComponent>(entity1))
		{
			ECS.get<CollisionCallbackComponent>(entity1).others.emplace_back(std::make_pair(entity2, categoryBits2));
		}
		else
		{
			auto& callbackData = ECS.assign<CollisionCallbackComponent>(entity1);
			callbackData.selfCatagory = categoryBits1;
			callbackData.others.emplace_back(std::make_pair(entity2, categoryBits2));
		}
		
		if (ECS.has<CollisionCallbackComponent>(entity2))
		{
			ECS.get<CollisionCallbackComponent>(entity2).others.emplace_back(std::make_pair(entity1, categoryBits1));
		}
		else
		{
			auto& callbackData = ECS.assign<CollisionCallbackComponent>(entity2);
			callbackData.selfCatagory = categoryBits2;
			callbackData.others.emplace_back(std::make_pair(entity1, categoryBits1));
		}
		
	}
private:
	entt::DefaultRegistry& ECS;
};
