#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <SFML/Graphics.hpp>

#include "../engine/components.hpp"

namespace Enemy
{
    namespace Fire
    {
        void create(const sf::Vector2f& position, EntityID player_id) noexcept;
        void create(const sf::Vector2f& position) noexcept;

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Fire

    namespace Goomba 
    {
        void create(const sf::Vector2f& position) noexcept;

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept;
            void setupUpdateFunction(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Goomba
} // namespace Enemy

#endif