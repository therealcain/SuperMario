#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SFML/Graphics.hpp>
#include <string_view>

#include "components.hpp"

class Window {
public:
	Window();
	void eventHandler() noexcept;
	void clear() noexcept;
	void display() noexcept;
	void updateCamera(EntityID player_id) noexcept;

protected:
	static constexpr unsigned int WIDTH = 800;
	static constexpr unsigned int HEIGHT = 600;
	sf::RenderWindow window;
	sf::View view;

private:
	static constexpr unsigned int DEFAULT_FPS = 45;
	static constexpr std::string_view TITLE = "Super Mario";
};

#endif