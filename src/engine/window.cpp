#include "window.hpp"

Window::Window() {
	window.create(sf::VideoMode(WIDTH, HEIGHT), TITLE.data());
	window.setFramerateLimit(DEFAULT_FPS);
}

void Window::eventHandler() noexcept {
	
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