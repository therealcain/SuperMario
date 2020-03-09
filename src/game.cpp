#include "game.hpp"
#include "engine/manager.hpp"

#include "entities/block.hpp"
#include "entities/coin.hpp"

#include <iostream>

Game::Game() 
	: render(Window::window)
{
	Entity::Block::create(sf::Vector2f(100, 100), Enum::Block::EMPTY);
	Entity::Block::create(sf::Vector2f(23, 400),  Enum::Block::BRICK);
	Entity::Block::create(sf::Vector2f(65, 200),  Enum::Block::QUESTION_MARK);

	Entity::Coin::create(sf::Vector2f(54, 58));
}

bool Game::run() noexcept {
	while (window.isOpen()) {
		Window::eventHandler();
		Window::clear();

		render.drawAll();
		System::Game::updateAll();

		Window::display();
	}

	return EXIT_SUCCESS;
}