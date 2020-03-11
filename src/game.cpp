#include "game.hpp"
#include "engine/manager.hpp"

#include "entities/entities.hpp"
#include "entities/player.hpp"
#include "entities/enemies.hpp"

#include <iostream>

Game::Game() 
	: render(Window::window)
{
	Entity::Player::create(sf::Vector2f(180, 50), Enum::Mature::TEENAGE); // index 0
	Enemy::Goomba::create(sf::Vector2f(250, 50));

	for(size_t i = 1; i < 20; i++) {
		Entity::Block::create(sf::Vector2f(140 + ( 15 * i), 220), Enum::Block::EMPTY);
	}
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