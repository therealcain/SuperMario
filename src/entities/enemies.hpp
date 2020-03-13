#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <SFML/Graphics.hpp>

namespace Enemy
{
    namespace Goomba 
    {
        void create(const sf::Vector2f& position) noexcept;
    } // namespace Goomba
} // namespace Enemy

#endif