#pragma once
#include <vector>
#include "Button.h"
class Menu : public CollidableUI
{
public:
	Menu(b2World& box2DEngine, const b2Vec2& worldPos, float width, float height, const std::shared_ptr<Surface> surf)
		:
		CollidableUI(box2DEngine, worldPos, width, height, false),
		width(width * Camera::scalePixel),
		height(height* Camera::scalePixel),
		surf(surf)
	{
		getBody().SetUserData(this);
	}
	void Draw(Graphics& gfx) const
	{
		const auto drawPos = ConvertToScreenPos(getBody().GetPosition(), Camera::scalePixel, Camera::coordinateOffsetX, Camera::coordinateOffsetY);
		gfx.DrawRectDim(drawPos, (int)width, (int)height, Colors::Gray);
		gfx.DrawSprite(drawPos.x - int(width / 2), drawPos.y - int(height / 2), *surf, SpriteEffect::Copy{});
		if (isUpdateButton)
		{
			for (auto& b : btns)
			{
				b->Draw(gfx);
			}
		}
	}
	void Update(float dt, Mouse& mouse)
	{
		if (GetMouseState())
		{
			if (isUpdateButton)
			{
				for (auto& b : btns)
				{
					b->UpdateMouseIn(dt, mouse);
				}
			}
			while (!mouse.IsEmpty())
			{
				auto e = mouse.Read().GetType();
			}
		}
		else
		{
			for (auto& b : btns)
			{
				b->UpdateMouseLeave(dt);
			}
		}
	}
	void AddButton(Button* btn)
	{
		btns.emplace_back(btn);
	}
	void RemoveCollision()
	{
		for (auto& b : btns)
		{
			b->RemoveCollision();
		}
		getBody().SetActive(false);
	}
	void EnableCollision()
	{
		for (auto& b : btns)
		{
			b->EnableCollision();
		}
		getBody().SetActive(true);
	}
	void DisableButton()
	{
		if (isUpdateButton)
		{
			for (auto& b : btns)
			{
				b->RemoveCollision();
			}
			isUpdateButton = false;
		}
	}
	void EnableButoon()
	{
		if (!isUpdateButton)
		{
			for (auto& b : btns)
			{
				b->EnableCollision();
			}
			isUpdateButton = true;
		}
	}
private:
	bool isUpdateButton = true;
	const float width;
	const float height;
	const std::shared_ptr<Surface> surf;
	std::vector<Button*> btns;
};