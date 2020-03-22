#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <SFML/Graphics.hpp>

#include "../engine/components.hpp"

namespace Enemy
{
    namespace Fire
    {
        void create(const sf::Vector2f& position, EntityID player_id, Enum::Direction start_direction) noexcept;

        namespace Helper
        {
            void checkMovement(EntityID id) noexcept;
            void checkDeath(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Fire

    namespace Goomba
    {
        void create(const sf::Vector2f& position) noexcept;
    
        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept;
        }
    }

    namespace Spiny
    {
        void create(const sf::Vector2f& position) noexcept;
    
        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept;
            void checkCorrectAnimation(EntityID id) noexcept;
        }
    }

    namespace Helper
    {
        void setupBaseType(EntityID id, const sf::Vector2f& position, Enum::Type enemy_type) noexcept;
        void setupComponents(EntityID id) noexcept;
        void standardMovement(EntityID id) noexcept;
    }
} // namespace Enemy

#endif