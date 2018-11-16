#pragma once
#include "Graphics.h"
#include "TransformComponent.h"
#include "MeshComponent.h"
#include "TexCoordsComponent.h"
#include "entt/entt.hpp"
class DrawSystem
{
public:
	void Draw(entt::DefaultRegistry& ECS, Graphics& gfx, sf::Texture& texture)
	{
		ECS.view<TransformComponent, MeshComponent, TexCoordsComponent>().each([&](auto & entity, TransformComponent& trans, MeshComponent& mesh, TexCoordsComponent& texCoords) {
			sf::VertexArray quad = sf::VertexArray( sf::Quads, 4 );

			sf::Transform T;
			T.translate(trans.position);
			sf::Transform R;
			R.rotate(trans.rotation);

			sf::Transform M = T * R;

			quad[0].position = M.transformPoint(mesh.point01);
			quad[1].position = M.transformPoint(mesh.point02);
			quad[2].position = M.transformPoint(mesh.point03);
			quad[3].position = M.transformPoint(mesh.point04);

			quad[0].texCoords = texCoords.point01;
			quad[1].texCoords = texCoords.point02;
			quad[2].texCoords = texCoords.point03;
			quad[3].texCoords = texCoords.point04;
			gfx.Draw(quad, texture);
		});
	}
};