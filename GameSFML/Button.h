#pragma once
#include <functional>
#include <map>
#include "CollidableUI.h"
class Button : public CollidableUI
{
public:
	Button(b2World& box2DEngine, const b2Vec2& worldPos, float width, float height,bool isCircle = false)
		:
		CollidableUI(box2DEngine, worldPos, width, height, isCircle),
		width(width * Camera::scalePixel),
		height(height* Camera::scalePixel),
		surf(surf)
	{
		getBody().SetUserData(this);
	}
	void Draw(Graphics& gfx) const
	{
		Color color;
		if (GetMouseState())
		{
			if (disable)
			{
				color = Colors::Black;
			}
			else
			{
				color = Colors::Green;
			}
		}
		else
		{
			if (disable)
			{
				color = Colors::Black;
			}
			else
			{
				color = Colors::Yellow;
			}
		}
		const auto drawPos = ConvertToScreenPos(getBody().GetPosition(), Camera::scalePixel, Camera::coordinateOffsetX, Camera::coordinateOffsetY);
		gfx.DrawRectDim(drawPos, (int)width, (int)height, color);
		if (surf)
		{
			gfx.DrawSprite(drawPos.x - int(width / 2), drawPos.y - int(height / 2), *surf, SpriteEffect::AlphaBlendBaked{});
		}
	}
	void UpdateMouseIn(float dt, Mouse& mouse)
	{
		if (disable)
		{
			UpdateDisableState(dt);
		}
		else
		{
			if (GetMouseState())
			{
				SwitchMouseCommand(mouse);
			}
		}
	}
	void UpdateMouseLeave(float dt)
	{
		if (disable)
		{
			UpdateDisableState(dt);
		}
	}
	void AddEventListener(Mouse::Event::Type type, std::function<void()> pFunc)
	{
		handlers.emplace(type, pFunc);
	}
	void Enable()
	{
		disable = false;
		timer = 0.0f;
	}
	void Disable(float duration)
	{
		disable = true;
		this->duration = duration;
	}
	void setColor(const Color& color)
	{
		c = color;
	}
	void SetSprite(const std::shared_ptr<Surface> surface)
	{
		surf = surface;
	}
	void RemoveCollision()
	{
		getBody().SetActive(false);
		timer = 0.0f;
		duration = 0.0f;
		disable = false;
		MouseLeave();
	}
	void EnableCollision()
	{
		getBody().SetActive(true);
	}
private:
	void SwitchMouseCommand(Mouse& mouse)
	{
		while (!mouse.IsEmpty())
		{
			auto e = mouse.Read().GetType();
			auto h = handlers.find(e);
			if (h != handlers.end())
			{
				h->second();
			}
		}
	}
	void UpdateDisableState(float dt)
	{
		timer += dt;
		if (timer >= duration)
		{
			Enable();
		}
	}
private:
	std::map<Mouse::Event::Type, std::function<void()>> handlers;
	const float width;
	const float height;
	Color c;
	std::shared_ptr<Surface> surf;
	bool disable = false;
	float timer = 0.0f;
	float duration = 1.0f;
};