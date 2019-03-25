#pragma once
#include "entt/entt.hpp"
struct TransitionStateComponent 
{
	//this will use by TransitionStateSystem, and this system will use multi-thread don't remove or add new Component
	entt::Delegate<void(uint32_t, entt::DefaultRegistry&)> myDelegate;
};