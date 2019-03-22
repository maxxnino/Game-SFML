#pragma once
#include "ISystem.h"
#include "Component/PhysicComponent.h"
#include "Locator.h"
#include "Component/StaticObjectSpawnInfo.h"
#include "Component/GameplayTags.h"
class SpawnStaticObjectSystem : public ISystem
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto& gfx = Locator::Graphic::ref();
		ECS.view<StaticObjectSpawnInfo>().each([&ECS, &gfx](auto entity, StaticObjectSpawnInfo &info) {
			const b2Vec2 worldSize = gfx.GetWorldSize({ info.width, info.height });
			const auto pos = gfx.ScreenToWorldPos(info.pos);
			

			b2BodyDef bodyDef;
			bodyDef.type = b2_staticBody;
			bodyDef.position = pos;

			b2PolygonShape boxShape;
			boxShape.SetAsBox(worldSize.x, worldSize.y);

			b2FixtureDef fixtureDef;
			fixtureDef.shape = &boxShape;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.restitution = 1.0f;

			ECS.assign<PhysicComponent>(entity, entity, bodyDef, fixtureDef);
			ECS.assign<PhysicDebug>(entity);
		});
		ECS.reset<StaticObjectSpawnInfo>();
	}
};