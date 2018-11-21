#pragma once
#include "PhysicComponent.h"
#include "HashStringManager.h"
#include "AnimationComponent.h"
#include "entt/entt.hpp"

class PlayerControlSystem
{
	enum PlayerState
	{
		Down,
		Left,
		Right,
		Up,
	};
public:
	void Update(entt::DefaultRegistry& ECS, float dt, Keyboard& kbd, Mouse& mouse)
	{
		bool fire = false;
		sf::Vector2f dir = { 0.0f,0.0f };
		PlayerState state = PlayerState::Right;
		//const auto mousePos = ScreenToWorldPos((VecF)mouse.GetPos());
		if (kbd.KeyIsPressed(sf::Keyboard::A))
		{
			dir.x = -1.0f;
			state = PlayerState::Left;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::D))
		{
			dir.x = 1.0f;
			state = PlayerState::Right;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::W))
		{
			dir.y = 1.0f;
			state = PlayerState::Up;
		}
		if (kbd.KeyIsPressed(sf::Keyboard::S))
		{
			dir.y = -1.0f;
			state = PlayerState::Down;
		}
		if (mouse.LeftIsPressed())
		{
			fire = true;
		}
		if (mouse.RightIsPressed())
		{
			fire = false;
		}
		ECS.view<PhysicComponent, GroupAnimation, entt::label<HS::InputComponent>>().each([&](const auto, PhysicComponent& physicComponent, GroupAnimation& animate,const auto) {
			animate.iAnimation = static_cast<int>(state);
			b2Vec2 vel = physicComponent.body->GetLinearVelocity();
			b2Vec2 velChange = b2Vec2(10.0f * dir.x - vel.x, 10.0f * dir.y - vel.y);
			physicComponent.body->ApplyLinearImpulseToCenter(physicComponent.body->GetMass() * velChange, true);

			/*const b2Vec2 pos = physicComponent.body->GetPosition();
			b2Vec2 dir = inputComponent.mousePos - pos;
			dir.Normalize();
			if (fire)
			{
				auto bulletEntity = ECSManager.create();
				ECSManager.assign<BulletDataComponent>(bulletEntity, pos, dir);
			}*/
		});
	}
};