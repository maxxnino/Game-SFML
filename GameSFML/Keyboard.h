/******************************************************************************************
 *	Chili DirectX Framework Version 16.07.20											  *
 *	Keyboard.h																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#pragma once
#include <queue>
#include <bitset>
#include "SFML/Graphics.hpp"
class Keyboard
{
	friend class MainWindow;
public:
	class Event
	{
	public:
		enum class Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		sf::Keyboard::Key code;
	public:
		Event()
			:
			type(Type::Invalid),
			code(sf::Keyboard::Key::Unknown)
		{}
		Event(Type type, sf::Keyboard::Key code)
			:
			type(type),
			code(code)
		{}
		bool IsPress() const
		{
			return type == Type::Press;
		}
		bool IsRelease() const
		{
			return type == Type::Release;
		}
		bool IsValid() const
		{
			return type != Type::Invalid;
		}
		sf::Keyboard::Key GetCode() const
		{
			return code;
		}
	};
public:
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	bool KeyIsPressed(sf::Keyboard::Key keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	char ReadChar();
	bool CharIsEmpty() const;
	void FlushKey();
	void FlushChar();
	void Flush();
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnabled() const;
private:
	void OnKeyPressed(sf::Keyboard::Key keycode);
	void OnKeyReleased(sf::Keyboard::Key keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T>
	void TrimBuffer(std::queue<T>& buffer);
private:
	static constexpr int bufferSize = 4u;
	bool autorepeatEnabled = false;
	std::bitset<sf::Keyboard::Key::KeyCount> keystates;
	std::queue<Event> keybuffer;
	std::queue<char> charbuffer;
};
