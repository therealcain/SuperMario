#include "game.hpp"
#include "engine/manager.hpp"

#include "entities/block.hpp"
#include "entities/coin.hpp"
#include "entities/cloud.hpp"
#include "entities/player.hpp"

#include <iostream>

Game::Game() 
	: render(Window::window)
{
	Entity::Block::create(sf::Vector2f(100, 100), Enum::Block::EMPTY);
	Entity::Block::create(sf::Vector2f(23, 400),  Enum::Block::BRICK);
	Entity::Block::create(sf::Vector2f(65, 200),  Enum::Block::QUESTION_MARK);

	Entity::Coin::create(sf::Vector2f(54, 58));
	Entity::Coin::create(sf::Vector2f(300, 400));
	Entity::Cloud::create(sf::Vector2f(555, 300));

	Entity::Player::create(sf::Vector2f(200, 200), Enum::Mature::CHILD);
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