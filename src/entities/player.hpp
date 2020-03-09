#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"

namespace Entity 
{
    namespace Player 
    {
        void create(sf::Vector2f&& position, Enum::Mature maturity) noexcept;
    } // namespace Player
} // namespace Entity

#endif