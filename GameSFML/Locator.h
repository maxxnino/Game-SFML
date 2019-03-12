#pragma once
#include "entt/entt.hpp"
#include "Box2D/Box2D.h"
#include "Codex.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"
#include <random>
struct Locator
{
	using Physic = entt::ServiceLocator<b2World>;
	using Graphic = entt::ServiceLocator<Graphics>;
	using ECS = entt::ServiceLocator<entt::DefaultRegistry>;
	using Random = entt::ServiceLocator<std::mt19937>;
	using Mouse = entt::ServiceLocator<Mouse>;
	using Keyboard = entt::ServiceLocator<Keyboard>;
};