#pragma once
#include "Graphics.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "TexCoordsComponent.h"
#include "TextureManager.h"
#include "entt/entt.hpp"

class DrawSystem
{
public:
	void Draw(entt::DefaultRegistry& ECS, Graphics& gfx, TextureManager& manager)
	{
		/*ECS.persistent_view<int, char>();
		entt::Registry<> registry;
		entt::registry<> registry;*/

		const auto size = ECS.view<entt::label<"Enemy01"_hs>>().size();
		int count = 0;
		sf::VertexArray quad = sf::VertexArray(sf::Quads, 4 * size);
		const MeshComponent& mesh = manager.GetMesh("Enemy01");

		ECS.view<TransformComponent, TexCoordsComponent,entt::label<"Enemy01"_hs>>().each([&](const auto, TransformComponent& trans, TexCoordsComponent& texCoords, const auto) {
			const auto amout = count * 4;
			sf::Transform T;
			T.translate(trans.position);
			T.rotate(trans.rotation);

			quad[0 + amout].position = T.transformPoint(mesh.point01);
			quad[1 + amout].position = T.transformPoint(mesh.point02);
			quad[2 + amout].position = T.transformPoint(mesh.point03);
			quad[3 + amout].position = T.transformPoint(mesh.point04);

			quad[0 + amout].texCoords = texCoords.point01;
			quad[1 + amout].texCoords = texCoords.point02;
			quad[2 + amout].texCoords = texCoords.point03;
			quad[3 + amout].texCoords = texCoords.point04;
			count++;
		});
		gfx.Draw(quad, manager.GetTexture("Enemy01"));
	}
};