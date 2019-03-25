#pragma once
#include "../System/ISystemECS.h"
#include "../Component/PhysicComponent.h"
#include "../Locator.h"
#include "../Component/StaticObjectSpawnInfo.h"
#include "../Component/GameplayTags.h"
class SpawnStaticObjectSystem : public ISystemECS
{
	struct OverlapQuerySelector final : public b2QueryCallback
	{
		bool ReportFixture(b2Fixture* fixture) final
		{
			const auto pos = fixture->GetBody()->GetPosition();
			if (aabb.upperBound.x > pos.x && aabb.upperBound.y > pos.y && aabb.lowerBound.x < pos.x && aabb.lowerBound.y < pos.y)
			{
				const auto entity = fixture->GetBody()->GetUserEntity();
				if (entity != exclutedEntity)
				{
					foundBodies.emplace_back(entity);
				}
				
			}
			return true;//keep going to find all fixtures in the query area
		}
		std::vector<uint32_t> foundBodies;
		b2AABB aabb;
		uint32_t exclutedEntity;
	};
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto& gfx = Locator::Graphic::ref();
		//spawn static object from tile
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

		//spawn custom object
		ECS.view<StaticCustomObjectSpawnInfo>().each([&ECS, &gfx, this](auto entity, StaticCustomObjectSpawnInfo &info) {
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

			//check if overlap with any tile object and delete the tile
			b2AABB aabb;
			aabb.lowerBound = pos - worldSize;
			aabb.upperBound = pos + worldSize;

			overlapQuery.foundBodies.clear();
			overlapQuery.aabb = aabb;
			overlapQuery.exclutedEntity = entity;
			Locator::Physic::ref().QueryAABB(&overlapQuery, aabb);

			for (auto e : overlapQuery.foundBodies)
			{
				ECS.assign<Overlaped>(e);
			}
		});

		ECS.reset<StaticCustomObjectSpawnInfo>();
		ECS.destroy<Overlaped>();
	}
private:
	OverlapQuerySelector overlapQuery;
};