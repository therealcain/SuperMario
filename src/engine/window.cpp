#include "window.hpp"
#include "manager.hpp"
#include "system.hpp"
#include "helpers/values.hpp"

Window::Window() 
	: window(sf::VideoMode(WIDTH, HEIGHT), TITLE.data()), view(window.getDefaultView())
{
	window.setFramerateLimit(DEFAULT_FPS);
}

void Window::eventHandler() noexcept 
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
	}
}

void Window::clear() noexcept {
	window.clear(sf::Color::Cyan);
}

void Window::display() noexcept {
	window.display();
}

void Window::updateCamera(EntityID player_id) noexcept
{
	if(Manager::canAccess(player_id)) {
		const sf::Vector2f& objectPosition = System::Base::getSprite(player_id).getPosition();
		const bool playerRunning		   = System::Movement::getRunning(player_id);

		if(System::Movement::getMoving(player_id)) 
		{	
			if(objectPosition.x >= WIDTH / 2) 
			{
				if(System::Movement::getLookingDirection(player_id) == Enum::Direction::RIGHT)
				{
					if(playerRunning) {
						view.move(sf::Vector2f(SHIFTING_PLAYER_SPEED, 0));
					} else {
						view.move(sf::Vector2f(PLAYER_SPEED, 0));
					}
				} else if(System::Movement::getLookingDirection(player_id) == Enum::Direction::LEFT)
				{
					if(playerRunning) {
						view.move(sf::Vector2f(-SHIFTING_PLAYER_SPEED, 0));
					} else {
						view.move(sf::Vector2f(-PLAYER_SPEED, 0));
					}
				}
			}
		}

		window.setView(view);
	}
}