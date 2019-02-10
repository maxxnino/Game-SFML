#pragma once
#include "Graphics.h"
#include "PhysicComponent.h"
#include "Codex.h"
#include "AnimationComponent.h"
#include "entt/entt.hpp"
#include "HashStringManager.h"
#include <iostream>
#include <execution>
class CullingObject
{
	//query for select by point
	class CullingQuerySelector : public b2QueryCallback
	{
	public:
		bool ReportFixture(b2Fixture* fixture) final
		{
			foundBodies.emplace_back(fixture->GetBody()->GetUserEntity());
			return true;//keep going to find all fixtures in the query area
		}
		void Sort()
		{
			std::sort(std::execution::par_unseq, foundBodies.begin(), foundBodies.end());
		}
		std::vector<uint32_t> foundBodies;
	};
public:
	const std::vector<uint32_t>& GetEntityList(b2World& box2DEngine, b2Vec2 p01, b2Vec2 p02)
	{
		cullingQueryCallback.foundBodies.clear();
		b2AABB aabb;
		if (p01.x > p02.x)
		{
			std::swap(p01.x, p02.x);
		}
		if (p01.y > p02.y)
		{
			std::swap(p01.y, p02.y);
		}
		aabb.lowerBound = p01;
		aabb.upperBound = p02;
		box2DEngine.QueryAABB(&cullingQueryCallback, aabb);
		//cullingQueryCallback.Sort();
		return cullingQueryCallback.foundBodies;
	}
private:
	CullingQuerySelector cullingQueryCallback;
};
constexpr float radToDeg = 180.0f / 3.14f;
class DrawSystem
{
public:
	void Draw(entt::DefaultRegistry& ECS, Graphics& gfx, Codex& codex)
	{
		ECS.view<sf::Sprite, PhysicComponent, entt::label<HS::Drawable>>().each([&](const auto entity, sf::Sprite& sprite, const PhysicComponent& physic, const auto) {
			
			sprite.setPosition(gfx.GetDrawPosition(physic.body->GetPosition()));
			sprite.setRotation(physic.body->GetAngle() * radToDeg);
			gfx.DrawSprite(sprite);
		});

		ECS.view<entt::label<HS::Drawable>>().each([&](const auto entity, auto) {
			ECS.remove<entt::label<HS::Drawable>>(entity);
		});
	}
	void Update(entt::DefaultRegistry& ECS, Graphics& gfx, b2World& box2DEngine)
	{
		auto viewport = gfx.GetViewport();
		auto entities = cullingObject.GetEntityList(box2DEngine, viewport.first, viewport.second);
		//std::cout << entities.size() << "\n";
		for (auto e : entities)
		{
			ECS.assign<entt::label<HS::Drawable>>(e);
		}
	}
private:
	/*template <typename entt::HashedString::hash_type value>
	void DrawByHashString(entt::DefaultRegistry& ECS, Graphics& gfx, TextureManager& manager, const entt::HashedString& hs)
	{
		auto view = ECS.view<entt::label<HashStringManager::Drawable>, PhysicComponent, entt::label<value>>(entt::persistent_t{});
		sf::VertexArray quad = sf::VertexArray(sf::Quads, 4 * view.size());
		const ModelComponent& mesh = manager.GetMesh(hs);

		int count = 0;
		view.each([&](const auto entity, const auto, PhysicComponent& physic, const auto) {
			const auto amout = count * 4;
			sf::Transform T;
			T.translate(gfx.GetDrawPosition(physic.body->GetPosition()));
			T.rotate(physic.body->GetAngle());

			quad[0 + amout].position = T.transformPoint(mesh.meshP01);
			quad[1 + amout].position = T.transformPoint(mesh.meshP02);
			quad[2 + amout].position = T.transformPoint(mesh.meshP03);
			quad[3 + amout].position = T.transformPoint(mesh.meshP04);

			quad[0 + amout].texCoords = mesh.texCoord01;
			quad[1 + amout].texCoords = mesh.texCoord02;
			quad[2 + amout].texCoords = mesh.texCoord03;
			quad[3 + amout].texCoords = mesh.texCoord04;
			count++;
		});
		for (auto e : view)
		{
			ECS.remove<entt::label<HashStringManager::Drawable>>(e);
		}
		gfx.Draw(quad, manager.GetTexture(hs));
	}*/
private:
	CullingObject cullingObject;
};