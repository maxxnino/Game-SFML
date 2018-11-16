#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"
class MainWindow
{
public:
	MainWindow()
		:
		window(sf::VideoMode(1280, 720), "SFML Game"),
		game(*this)
	{
		//enable v-sync
		window.setVerticalSyncEnabled(true);
		//disable key auto repeat
		window.setKeyRepeatEnabled(false);
	}
	void Update()
	{
		while (window.isOpen())
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
					sf::FloatRect visibleArea(0.f, 0.f, (float)event.size.width, (float)event.size.height);
					window.setView(sf::View(visibleArea));
					break;
				}
					/*---------------------Window Event-------------------*/
				default:
					break;
				}
			}
			game.Go();
		}
	}
public:
	Mouse mouse;
	Keyboard kbd;
	sf::RenderWindow window;
private:
	Game game;	
};