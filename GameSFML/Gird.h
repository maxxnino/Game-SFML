#pragma once
#include "SFML/Graphics.hpp"
#include <assert.h>
#include <iostream>
#include "Codex.h"
#include "HashStringDataBase.h"
class Grid : public sf::Drawable, public sf::Transformable
{
public:
	template<typename Loader>
	void LoadFromFile(Loader& loader)
	{
		const GridResource resource = loader.GetGridResource(Database::GridMap);
		if (resource.tileSize <= 0 || resource.gridW <= 0 || resource.gridH <= 0 || resource.tileTexture == nullptr)
		{
			assert(false);
			return;
		}
		tileSize = resource.tileSize;
		gridW = resource.gridW;
		gridH = resource.gridH;
		tileTexture = resource.tileTexture;

		// resize the vertex array to fit the level size
		m_vertices.setPrimitiveType(sf::Quads);
		drawGid.setPrimitiveType(sf::Quads);
		m_vertices.resize(gridW * gridH * 4);
		
		for (int i = 0; i < gridW; ++i)
			for (int j = 0; j < gridH; ++j)
			{

				// get a pointer to the current tile's quad
				sf::Vertex* quad = &m_vertices[(i + j * gridW) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(float(i * tileSize), float(j * tileSize));
				quad[1].position = sf::Vector2f(float((i + 1) * tileSize), float(j * tileSize));
				quad[2].position = sf::Vector2f(float((i + 1) * tileSize), float((j + 1) * tileSize));
				quad[3].position = sf::Vector2f(float(i * tileSize), float((j + 1) * tileSize));

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(0.0f, 0.0f);
				quad[1].texCoords = sf::Vector2f((float)tileSize, 0.0f);
				quad[2].texCoords = sf::Vector2f((float)tileSize, (float)tileSize);
				quad[3].texCoords = sf::Vector2f(0.0f, (float)tileSize);
			}
	}
	void Culling(sf::Vector2f topLeft, sf::Vector2f rightBottom)
	{
		int left = 0, right = 0, top = 0, bottom = 0;
		//get top left point of view
		topLeft -= getPosition();
		left = int(topLeft.x / tileSize);
		top = int(topLeft.y / tileSize);

		//get bottom right point of view
		rightBottom -= getPosition();
		right = 1 + int(rightBottom.x / tileSize);
		bottom = 1 + int(rightBottom.y / tileSize);

		//clamp these to fit into array bounds:
		left = std::max(0, std::min(left, gridW));
		top = std::max(0, std::min(top, gridH));
		right = std::max(0, std::min(right, gridW));
		bottom = std::max(0, std::min(bottom, gridH));

		//set texture and draw visible chunks:
		drawGid.resize((right - left) * (bottom - top) * 4);
		int iDraw = 0;
		for (int i = left; i < right; ++i)
		{
			for (int j = top; j < bottom; ++j)
			{
				const int cur = (i + j * gridW) * 4;

				// define its 4 corners
				drawGid[iDraw] = m_vertices[cur];
				drawGid[iDraw + 1] = m_vertices[cur + 1];
				drawGid[iDraw + 2] = m_vertices[cur + 2];
				drawGid[iDraw + 3] = m_vertices[cur + 3];
				iDraw += 4;
			}
		}
		std::cout << "Vertices: " << drawGid.getVertexCount() << ", Tiles: " << (right - left) * (bottom - top) << std::endl;
	}
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const final
	{
		if (tileSize <= 0 || gridW <= 0 || gridH <= 0 || tileTexture == nullptr)
		{
			assert(false);
			return;
		}
		states.texture = tileTexture;
		states.transform *= getTransform();
		target.draw(drawGid, states);
	}
private:
	int tileSize = 40, gridW = 0, gridH = 0;
	const sf::Texture* tileTexture = nullptr;
	sf::VertexArray m_vertices;
	sf::VertexArray drawGid;
};