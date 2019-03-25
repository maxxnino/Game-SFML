#pragma once
#include <entt/core/hashed_string.hpp>
namespace Database
{
#pragma warning(push)
#pragma warning(disable:4307) // disable warning C4307: '*': integral constant overflow
	constexpr auto GridMap = "Data\\Json\\map20x20.json"_hs;
	constexpr auto TEnemy = "Data\\Images\\enemy_02.png"_hs;
	constexpr auto PlayerAnimation = "Data\\Json\\playerAnimation.json"_hs;
	constexpr auto BulletAnimation = "Data\\Json\\bulletAnimation.json"_hs;
	constexpr auto FontSplatch = "Data\\Fonts\\Adventure.otf"_hs;
#pragma warning(pop)
}