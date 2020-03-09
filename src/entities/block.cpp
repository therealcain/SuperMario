#include "block.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"

#include <iostream>

namespace Entity
{
    namespace Block
    {
        void create(const sf::Vector2f&& position, Enum::Block type) noexcept
        {
            EntityID currentID = Manager::create("assets/block.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type        = Enum::Type::BLOCK;
            Component::types[currentID] -> maybeBlock = type;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) {
                switch(type) {
                    case Enum::Block::EMPTY:
                        System::Animation::setFrames(currentID, int(Enum::Block::EMPTY), sf::IntRect(64, 0, 16, 16));
                    break;

                    case Enum::Block::BRICK:
                        System::Animation::setFrames(currentID, int(Enum::Block::BRICK), {
                            sf::IntRect(0, 16, 16, 16),
                            sf::IntRect(32, 0, 16, 16),
                            sf::IntRect(48, 0, 16, 16),
                            sf::IntRect(64, 0, 16, 16) 
                        });
                    break;

                    case Enum::Block::QUESTION_MARK:
                        System::Animation::setFrames(currentID, int(Enum::Block::QUESTION_MARK), {
                            sf::IntRect(16, 0, 16, 16),
                            sf::IntRect(16, 16, 16, 16),
                            sf::IntRect(32, 16, 16, 16),
                            sf::IntRect(64, 0, 16, 16) 
                        });
                    break;
                }
                
                System::Animation::setCurrentAnimation(currentID, int(type));
                System::Animation::setNextAnimationTimer(currentID, 100);
                System::Animation::setStopWhenFinished(currentID, STOP::TRUE);
                System::Animation::setAllowPlay(currentID, ALLOW::FALSE);
            }

        
            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) {
                System::Animation::play(id);
            };
        }   

        void hit(EntityID id) noexcept 
        {
            if(Manager::canAccess(id)) {
                System::Animation::setAllowPlay(id, ALLOW::TRUE);
            } 
        }
    } // namespace Block

} // namespace Entity