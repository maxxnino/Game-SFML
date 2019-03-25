#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PlayerControllerComponent.h"
#include "../Component/PhysicComponent.h"
#include "../Component/GameplayTags.h"
class PlayerUpdateSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto& gfx = Locator::Graphic::ref();
		ECS.view<PlayerControllerComponent, PhysicComponent>().each([&ECS, &gfx](auto entity, PlayerControllerComponent& controller, PhysicComponent& physic) {
			b2Vec2 vel = physic.body->GetLinearVelocity();
			b2Vec2 desiredVel{(float)controller.direction.x, (float)controller.direction.y };
			desiredVel.Normalize();
			desiredVel = 15.0f * desiredVel;
			physic.body->ApplyLinearImpulseToCenter(physic.body->GetMass() * (desiredVel - vel), true);

			if (controller.bIsShooting)
			{
				auto entityBullet = ECS.create();
				ECS.assign<LifeTimeComponent>(entityBullet, 5.0f, 0.0f);
				ECS.assign<HealthComponent>(entityBullet, 10.0f);
				//animation
				auto& animation = ECS.assign<AnimationComponent>(entityBullet, Locator::Codex::ref().GetAnimation(Database::BulletAnimation));
				b2CircleShape circle;
				//sprite
				{
					auto& sprite = ECS.assign<sf::Sprite>(entityBullet);
					sprite.setTexture(*animation.resource->texture);
					const auto textSize = 0.5f * sf::Vector2f((float)animation.resource->tileWidth, (float)animation.resource->tileHeight);
					sprite.setOrigin(textSize);
					circle.m_radius = textSize.x / gfx.scalePixel;
				}

				//physic
				const auto parentPosition = physic.body->GetPosition();
				auto direction = gfx.MouseToWorldPos(controller.mousePos) - parentPosition;
				direction.Normalize();
				b2BodyDef bodyDef;
				bodyDef.type = b2_dynamicBody;
				bodyDef.position = parentPosition + direction;
				

				b2FixtureDef fixtureDef;
				fixtureDef.shape = &circle;
				fixtureDef.filter.categoryBits = CollisionFillter::BULLET;
				fixtureDef.filter.maskBits = CollisionFillter::STATIC | CollisionFillter::ENEMY;
				//fixtureDef.isSensor = true;
				fixtureDef.density = 1.0f;
				fixtureDef.friction = 0.0f;
				fixtureDef.restitution = 1.0f;

				ECS.assign<PhysicDebug>(entityBullet);
				auto& bulletPhysic = ECS.assign<PhysicComponent>(entityBullet, entityBullet, bodyDef, fixtureDef);
				bulletPhysic.body->ApplyForceToCenter(5000.0f * direction, true);
				ECS.assign<CollisionRespondComponent>(entityBullet).myDelegate.connect<&CollisionRespond::Bullet>();
			}
		});
	}
};