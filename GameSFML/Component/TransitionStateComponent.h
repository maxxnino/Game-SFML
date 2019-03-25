#pragma once
#include "entt/entt.hpp"
struct TransitionStateComponent 
{
	//this will use by TransitionStateSystem, and this system will use multi-thread becareful about data race
	entt::SigH<void(uint32_t, entt::DefaultRegistry&)> signal;
};