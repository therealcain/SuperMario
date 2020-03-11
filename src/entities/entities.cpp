#include "entities.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"

#include <iostream>

namespace Entity
{
    // ---------------------------------------------------------- //
    // -------------------------- BLOCK ------------------------- //
    // ---------------------------------------------------------- //
    namespace Block
    {
        void create(const sf::Vector2f&& position, Enum::Block type) noexcept
        {
            EntityID currentID = Manager::create("assets/block.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type     = Enum::Type::BLOCK;
            Component::types[currentID] -> whatType = type;

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


    // ---------------------------------------------------------- //
    // -------------------------- CLOUD ------------------------- //
    // ---------------------------------------------------------- //
    namespace Cloud {
        void create(sf::Vector2f&& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/cloud.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::CLOUD;
        }
    } // namespace Cloud


    // ---------------------------------------------------------- //
    // -------------------------- COIN -------------------------- //
    // ---------------------------------------------------------- //
    namespace Coin {
        void create(sf::Vector2f&& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/coin.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::COIN;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) {
                static constexpr int animPos = 0;
                
                System::Animation::setFrames(currentID, animPos, {
                    sf::IntRect(0, 0, 10, 14),
                    sf::IntRect(16, 0, 10, 14),
                    sf::IntRect(32, 0, 10, 14),
                    sf::IntRect(48, 0, 10, 14),
                    sf::IntRect(3, 15, 3, 15),
                    sf::IntRect(17, 15, 7, 15),
                    sf::IntRect(35, 15, 4, 16),
                    sf::IntRect(53, 15, 1, 16)
                });

                System::Animation::setNextAnimationTimer(currentID, 100);
                System::Animation::setCurrentAnimation(currentID, animPos);
            }

            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) {
                System::Animation::play(id);
            };
        }

        inline void hit(EntityID id) noexcept
        {
            Manager::remove(id);
        }
    } // namespace Coin

} // namespace Entity