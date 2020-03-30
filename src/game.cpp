#include "game.hpp"
#include "engine/manager.hpp"

#include "entities/entities.hpp"
#include "entities/player.hpp"
#include "entities/enemies.hpp"

#include <iostream>

Game::Game() 
	: render(Window::window)
{
	Entity::Player::create(sf::Vector2f(180, 50), Enum::Mature::CHILD); // index 0
	Entity::Coin::create(sf::Vector2f(300, 160));

	for(size_t i = 1; i < 20; i++) {
		Entity::Block::create(sf::Vector2f(140 + ( 15 * i), 220), Enum::Block::EMPTY, std::nullopt);
	}

	Entity::Block::create(sf::Vector2f(220, 160), Enum::Block::QUESTION_MARK, Enum::Type::MUSHROOM);
	Entity::Block::create(sf::Vector2f(260, 160), Enum::Block::QUESTION_MARK, Enum::Type::FLOWER);
	Entity::Block::create(sf::Vector2f(180, 205), Enum::Block::EMPTY, std::nullopt);
	Enemy::Goomba::create(sf::Vector2f(350, 50));
	Enemy::Spiny::create(sf::Vector2f(300, 50));
}

bool Game::run() noexcept {
	while (window.isOpen()) {
		Window::eventHandler();
		Window::clear();

		render.drawAll();
		System::Game::updateAll();

		Window::updateCamera(/*player_id*/0);
		Window::display();
	}

	return EXIT_SUCCESS;
}