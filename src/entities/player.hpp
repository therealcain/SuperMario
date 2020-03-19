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
            void setupAnimation(EntityID id) noexcept;
            void setupUpdateFunction(EntityID id) noexcept;
            
            void startMovement(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction);   
            void checkJump(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction) noexcept;
            void checkShooting(EntityID id, Enum::Mature maturity);
            bool checkCrouch(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction) noexcept;
            bool checkMovementRight(EntityID id, float speed, Enum::Mature maturity, Enum::Direction looking_direction) noexcept;
            bool checkMovementLeft(EntityID id, float speed, Enum::Mature maturity, Enum::Direction looking_direction) noexcept;
            void IDLE(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction);

            float checkSpeed() noexcept;
            bool checkPlayerRunning(float speed) noexcept;
            bool checkCrouching(EntityID id) noexcept;

            sf::Texture changeMarioRednessColor(sf::Texture texture, const sf::Color& color) noexcept;
        }
    } // namespace Player
} // namespace Entity

#endif