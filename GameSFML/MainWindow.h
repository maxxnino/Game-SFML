#pragma once
#include <SFML/Graphics.hpp>
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
//#include <iostream>
class MainWindow
{
public:
	MainWindow()
		:
		window(sf::VideoMode(1280, 720), "SFML Game")
	{
		//enable v-sync
		window.setVerticalSyncEnabled(true);
		//disable key auto repeat
		window.setKeyRepeatEnabled(false);
	}
	bool Update()
	{
		if (window.isOpen())
		{
			ProcessEvent();
			return true;
		}
		return false;
	}	
private:
	void ProcessEvent()
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				/*----------------------Mouse Event---------------------*/
			case sf::Event::MouseMoved:
				mouse.OnMouseMove(event.mouseMove.x, event.mouseMove.y);
				break;
			case sf::Event::MouseButtonPressed:
			{
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mouse.OnLeftPressed();
					break;
				case sf::Mouse::Right:
					mouse.OnRightPressed();
					break;
				default:
					break;
				}
				break;
			}
			case sf::Event::MouseButtonReleased:
			{
				switch (event.mouseButton.button)
				{
				case sf::Mouse::Left:
					mouse.OnLeftReleased();
					break;
				case sf::Mouse::Right:
					mouse.OnRightReleased();
					break;
				default:
					break;
				}
				break;
			}
			case sf::Event::MouseWheelScrolled:
			{
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
					mouse.OnWheel(event.mouseWheelScroll.delta);
				break;
			}
			break;
			//mouse cursor has entered the window
			case sf::Event::MouseEntered:
				break;
				//mouse cursor has left the window
			case sf::Event::MouseLeft:
				mouse.Flush();
				break;
				/*-------------------Mouse Event----------------------*/

				/*-------------------Keyboard Event-------------------*/
			case sf::Event::KeyPressed:
				kbd.OnKeyPressed(event.key.code);
				break;
			case sf::Event::KeyReleased:
				kbd.OnKeyReleased(event.key.code);
				break;

				/*-------------------Keyboard Event-------------------*/

				/*--------------------Window Event--------------------*/
				//happen when user closed the program by many way
			case sf::Event::Closed:
				window.close();
				break;
				//hapen when user change active window like alt+tab
			case sf::Event::LostFocus:
				mouse.Flush();
				break;
				//hapen when user comeback to program
			case sf::Event::GainedFocus:
				break;
				// catch the resize events
			case sf::Event::Resized:
			{
				// update the view to the new size of the window
				auto view = window.getView();
				view.setSize(sf::Vector2f((float)event.size.width, (float)event.size.height));
				window.setView(view);
				break;
			}
			/*---------------------Window Event-------------------*/
			default:
				break;
			}
		}
		
	}
public:
	Mouse mouse;
	Keyboard kbd;
	sf::RenderWindow window;
};