#pragma once
#include "../Graphics.h"
#include "../Component/PhysicComponent.h"
#include "../System/IDrawSystem.h"
#include "../Locator.h"
#include "../Component/GameplayTags.h"
class DrawDebugSystem : public IDrawSystem
{
public:
	void Draw(Graphics& gfx) const final
	{
		if (Locator::ECS::empty()) return;

		auto& ECS = Locator::ECS::ref();
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
				rectangle.setFillColor(sf::Color(255, 0, 0, 128));
				gfx.Draw(rectangle);
				break;
			}
			case b2Shape::Type::e_circle:
			{
				sf::CircleShape shape;
				const auto radius = physic.body->GetFixtureList()->GetShape()->m_radius;
				shape.setRadius(radius * gfx.scalePixel);
				shape.setPosition(gfx.WorldToScreenPos(physic.body->GetPosition() + b2Vec2(-radius, radius)));
				shape.setFillColor(sf::Color(0, 0, 255, 128));
				gfx.Draw(shape);
				break;
			}
			default:
				break;
			}
		});
	}
};