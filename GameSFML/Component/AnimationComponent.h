#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
struct FramesInfo
{
	FramesInfo(float holdFrame) : holdFrame(holdFrame) {}
	std::vector<sf::IntRect> frames;
	const float holdFrame;
};
struct AnimationComponent
{
	AnimationComponent(const FramesInfo& frameInfo)
		:
		holdTime(frameInfo.holdFrame),
		maxFrame((uint32_t)frameInfo.frames.size()),
		frames(&(frameInfo.frames))
	{}
	float holdTime;
	float curFrameTime = 0.0f;
	uint32_t iCurFrame = 0;
	uint32_t maxFrame;
	const std::vector<sf::IntRect>* frames = nullptr;
};
enum AnimationState
{
	WALKING,
	STANDING
};
enum Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};