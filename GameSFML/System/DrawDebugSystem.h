#pragma once
#include "Graphics.h"
#include "Component/PhysicComponent.h"
#include "Locator.h"
#include "Component/GameplayTags.h"
class DrawDebugSystem
{
public:
	void Draw()
	{
		auto& ECS = Locator::ECS::ref();
		auto& gfx = Locator::Graphic::ref();
		ECS.view<Viewable, PhysicDebug, PhysicComponent>().each([&ECS, &gfx](auto entity, auto&, auto&, PhysicComponent &physic) {
			switch (physic.body->GetFixtureList()->GetShape()->m_type)
			{
			case b2Shape::Type::e_polygon:
			{
				sf::RectangleShape rectangle;
				const b2Vec2 boxExtents = static_cast<b2PolygonShape*>(physic.body->GetFixtureList()->GetShape())->m_vertices[2];
				const auto screenSize = 2.0f * gfx.GetScreenSize(boxExtents);
				rectangle.setPosition(gfx.WorldToScreenPos(physic.body->GetPosition() + b2Vec2(-boxExtents.x, boxExtents.y)));
				rectangle.setSize(screenSize);
				rectangle.setFillColor(sf::Color::Red);
				gfx.Draw(rectangle);
				break;
			}
			case b2Shape::Type::e_circle:
			{
				sf::CircleShape shape;
				const auto radius = physic.body->GetFixtureList()->GetShape()->m_radius;
				shape.setRadius(radius * gfx.scalePixel);
				shape.setPosition(gfx.WorldToScreenPos(physic.body->GetPosition() + b2Vec2(-radius, radius)));
				shape.setFillColor(sf::Color::Blue);
				gfx.Draw(shape);
				break;
			}
			default:
				break;
			}
		});
	}
};