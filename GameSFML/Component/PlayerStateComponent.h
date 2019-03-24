#pragma once
struct PlayerStateComponent
{
	enum State
	{
		Standing,
		Walking
	} state = State::Standing;
	enum Direction
	{
		Up = 0,
		Left = 1,
		Down = 2,
		Right = 3
	} direction = Direction::Left;
};