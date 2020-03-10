#include "player.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"
#include "../engine/helpers/functions.hpp"

namespace Entity 
{
    namespace Player 
    {
        void create(sf::Vector2f&& position, Enum::Mature maturity) noexcept
        {
            EntityID currentID = Manager::create("assets/mario.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type     = Enum::Type::MARIO;
            Component::types[currentID] -> whatType = maturity;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) 
            {   
                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(133, 99, 145, 114)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(83, 99, 95, 114)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
                    System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setNextAnimationTimer(currentID, 150);
                System::Animation::setAllowPlay(currentID, ALLOW::TRUE);
                System::Animation::setCurrentAnimation(currentID, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::CHILD));
            }

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) 
            {
                if(System::Movement::isPrepared(id))
                {
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                        System::Movement::moveRight(id, 1, Enum::Animation::WALK_RIGHT, Enum::Mature::CHILD);            
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                        System::Movement::moveLeft(id, 1, Enum::Animation::WALK_RIGHT, Enum::Mature::CHILD);
                    }
                }
            };
        }
    } // namespace Player
} // namespace Entity