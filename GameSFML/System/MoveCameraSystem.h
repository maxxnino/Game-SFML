#pragma once
#include "../System/ISystemECS.h"
#include "../Locator.h"
class MoveCameraSystem : public ISystemECS
{
public:
	void Update(entt::DefaultRegistry& ECS, float dt) final
	{
		auto& mouse = Locator::Mouse::ref();
		while (!mouse.IsEmpty())
		{
			const auto mouseEvent = mouse.Read();
			switch (mouseEvent.GetType())
			{
			case Mouse::Event::Type::RPress:
				engaged = true;
				lastPos = mouse.GetPos();
				break;
			case Mouse::Event::Type::RRelease:
				engaged = false;
				break;
			}

		}
		
		if (engaged)
		{
			const auto curPos = mouse.GetPos();
			Locator::Graphic::ref().MoveViewport(lastPos - mouse.GetPos());
			lastPos = curPos;
		}
	}
private:
	bool engaged = false;
	sf::Vector2i lastPos;
};