#pragma once
#include <entt/core/hashed_string.hpp>
namespace Database
{
#pragma warning(push)
#pragma warning(disable:4307) // disable warning C4307: '*': integral constant overflow
	constexpr auto GridMap = "Data\\Json\\map200x200.json"_hs;
	constexpr auto PlayerAnimation = "Data\\Json\\playerAnimation.json"_hs;
#pragma warning(pop)
}