#pragma once
#include "Graphics.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "TextureManager.h"
#include "entt/entt.hpp"
#include "HashStringManager.h"
class DrawSystem
{
public:
	DrawSystem(entt::DefaultRegistry& ECS)
	{
		Prepare<HashStringManager::Enemy01>(ECS);
		Prepare<HashStringManager::Enemy02>(ECS);
		Prepare<HashStringManager::Enemy03>(ECS);
	}
	void Draw(entt::DefaultRegistry& ECS, Graphics& gfx, TextureManager& manager)
	{	
		DrawByHashString<HashStringManager::Enemy01>(ECS, gfx, manager, HashStringManager::Enemy01);
		DrawByHashString<HashStringManager::Enemy02>(ECS, gfx, manager, HashStringManager::Enemy02);
		DrawByHashString<HashStringManager::Enemy03>(ECS, gfx, manager, HashStringManager::Enemy03);
	}
private:
	template <typename entt::HashedString::hash_type value>
	void DrawByHashString(entt::DefaultRegistry& ECS, Graphics& gfx, TextureManager& manager, const entt::HashedString& hs)
	{
		const auto size = ECS.view<entt::label<value>>().size();
		int count = 0;
		sf::VertexArray quad = sf::VertexArray(sf::Quads, 4 * size);
		const ModelComponent& mesh = manager.GetMesh(hs);
		ECS.view<TransformComponent, entt::label<value>>(entt::persistent_t{}).each([&](const auto, TransformComponent& trans, const auto) {
			const auto amout = count * 4;
			sf::Transform T;
			T.translate(trans.position);
			T.rotate(trans.rotation);

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
		gfx.Draw(quad, manager.GetTexture(hs));
	}
	template <typename entt::HashedString::hash_type value>
	void Prepare(entt::DefaultRegistry& ECS)
	{
		ECS.prepare<TransformComponent, entt::label<value>>();	
	}
};