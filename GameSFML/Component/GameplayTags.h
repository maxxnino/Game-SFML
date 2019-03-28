#pragma once
struct DeathTag {};
struct Viewable {};
struct UIViewable {};
struct PhysicDebug {};
struct Overlaped {};
struct CameraTracking {};
struct LifeTimeComponent
{
	float maxLifeTime = 0.0f;
	float curTime = 0.0f;
};