#include "player.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"

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
                System::Animation::setFrames(currentID, int(Enum::Animation::IDLE_RIGHT) + int(Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(133, 99, 145, 114)));

                System::Animation::setFrames(currentID, int(Enum::Animation::IDLE_LEFT)  + int(Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(83, 99, 95, 114)));

                System::Animation::setFrames(currentID, int(Enum::Animation::WALK_RIGHT) + int(Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
                    System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, int(Enum::Animation::WALK_LEFT)  + int(Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(currentID, int(Enum::Animation::RUN_RIGHT)  + int(Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, int(Enum::Animation::RUN_LEFT)   + int(Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setNextAnimationTimer(currentID, 150);
                System::Animation::setAllowPlay(currentID, ALLOW::FALSE);
                System::Animation::setCurrentAnimation(currentID, int(Enum::Animation::IDLE_RIGHT));
            }

            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) {
                
            };
        }
    } // namespace Player
} // namespace Entity