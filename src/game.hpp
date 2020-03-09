#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>

#include "engine/window.hpp"
#include "engine/system.hpp"

class Game : private Window {
public:
	Game();
	bool run() noexcept;

private:
	System::Render render;
};

#endif