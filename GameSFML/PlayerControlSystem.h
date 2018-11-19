#pragma once
#include "PhysicComponent.h"
#include "HashStringManager.h"
#include "entt/entt.hpp"

class PlayerControlSystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt, Keyboard& kbd, Mouse& mouse)
	{
		//bool fire = false;
		//sf::Vector2f dir = { 0.0f,0.0f };
		////const auto mousePos = ScreenToWorldPos((VecF)mouse.GetPos());
		//if (kbd.KeyIsPressed(sf::Keyboard::A))
		//{
		//	dir.x = -1.0f;
		//}
		//if (kbd.KeyIsPressed(sf::Keyboard::D))
		//{
		//	dir.x = 1.0f;
		//}
		//if (kbd.KeyIsPressed(sf::Keyboard::W))
		//{
		//	dir.y = 1.0f;
		//}
		//if (kbd.KeyIsPressed(sf::Keyboard::S))
		//{
		//	dir.y = -1.0f;
		//}
		//if (mouse.LeftIsPressed())
		//{
		//	fire = true;
		//}
		//if (mouse.RightIsPressed())
		//{
		//	fire = false;
		//}
		//ECS.view<PhysicComponent, entt::label<HashStringManager::InputComponent>>().each([&](const auto, PhysicComponent& physicComponent, const auto) {
		//	b2Vec2 vel = physicComponent.body->GetLinearVelocity();
		//	b2Vec2 velChange = b2Vec2(10.0f * dir.x - vel.x, 10.0f * dir.y - vel.y);
		//	physicComponent.body->ApplyLinearImpulseToCenter(physicComponent.body->GetMass() * velChange, true);

		//	/*const b2Vec2 pos = physicComponent.body->GetPosition();
		//	b2Vec2 dir = inputComponent.mousePos - pos;
		//	dir.Normalize();
		//	if (fire)
		//	{
		//		auto bulletEntity = ECSManager.create();
		//		ECSManager.assign<BulletDataComponent>(bulletEntity, pos, dir);
		//	}*/
		//});
	}
};