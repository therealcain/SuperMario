#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"
#include "../engine/helpers/enums.hpp"

namespace Entity 
{
    namespace Player 
    {
        void create(const sf::Vector2f& position, Enum::Mature maturity) noexcept;
    
        namespace Helper 
        {
            void startMovement(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction);   
        }
    } // namespace Player
} // namespace Entity

#endif