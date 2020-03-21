#ifndef ENEMIES_HPP
#define ENEMIES_HPP

#include <SFML/Graphics.hpp>

#include "../engine/components.hpp"

namespace Enemy
{
    namespace Fire
    {
        void create(const sf::Vector2f& position, EntityID player_id) noexcept;

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Fire

    void create(const sf::Vector2f& position, Enum::Type enemy_type) noexcept;

    namespace Helper
    {
        void checkErrors(Enum::Type enemy_type);
        void setupAnimation(EntityID id, Enum::Type type) noexcept;
        void setupUpdateFunction(EntityID id) noexcept;
        void standardMovement(EntityID id) noexcept;
    }
} // namespace Enemy

#endif