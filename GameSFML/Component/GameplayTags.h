#pragma once
struct DeathTag {};
struct SpawnEnemyInfo {};
struct Viewable {};
struct PhysicDebug {};
struct Overlaped {};
struct CameraTracking {};
struct ScreenBaseUI {};
struct WorldBaseUI {};
struct LifeTimeComponent
{
	float maxLifeTime = 0.0f;
	float curTime = 0.0f;
};