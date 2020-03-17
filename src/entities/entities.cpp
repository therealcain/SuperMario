#include "entities.hpp"
#include "../engine/system.hpp"

#include <iostream>

namespace Entity
{
    // ---------------------------------------------------------- //
    // -------------------------- BLOCK ------------------------- //
    // ---------------------------------------------------------- //
    namespace Block
    {
        void create(const sf::Vector2f& position, Enum::Block type) noexcept
        {
            EntityID currentID = Manager::create("assets/block.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type     = Enum::Type::BLOCK;
            Component::types[currentID] -> whatType = type;

            Block::Helper::setupAnimations(currentID, type);
            Block::Helper::setupUpdateFunction(currentID);
        }   

        namespace Helper
        {
            void setupAnimations(EntityID id, Enum::Block type) noexcept
            {
                Manager::addComponent<Component::Animation>(id, BE_NULL::FALSE);
                switch(type) {
                    case Enum::Block::EMPTY:
                        System::Animation::setFrames(id, int(Enum::Block::EMPTY), sf::IntRect(64, 0, 16, 16));
                    break;

                    case Enum::Block::BRICK:
                        System::Animation::setFrames(id, int(Enum::Block::BRICK), {
                            sf::IntRect(0, 16, 16, 16),
                            sf::IntRect(32, 0, 16, 16),
                            sf::IntRect(48, 0, 16, 16),
                            sf::IntRect(64, 0, 16, 16) 
                        });
                    break;

                    case Enum::Block::QUESTION_MARK:
                        System::Animation::setFrames(id, int(Enum::Block::QUESTION_MARK), {
                            sf::IntRect(16, 0, 16, 16),
                            sf::IntRect(16, 16, 16, 16),
                            sf::IntRect(32, 16, 16, 16),
                            sf::IntRect(64, 0, 16, 16) 
                        });
                    break;
                }
                
                System::Animation::setCurrentAnimation(id, int(type));
                System::Animation::setNextAnimationTimer(id, 100);
                System::Animation::setStopWhenFinished(id, true);
                System::Animation::setAllowPlay(id, false);
            }

            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id, BE_NULL::FALSE);
                Component::updates[id] = [](EntityID update_id) -> void {
                    System::Animation::play(update_id);
                };
            }
        } // namespace Helper
    } // namespace Block


    // ---------------------------------------------------------- //
    // -------------------------- CLOUD ------------------------- //
    // ---------------------------------------------------------- //
    namespace Cloud 
    {
        void create(const sf::Vector2f& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/cloud.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::CLOUD;
        }
    } // namespace Cloud


    // ---------------------------------------------------------- //
    // -------------------------- COIN -------------------------- //
    // ---------------------------------------------------------- //
    namespace Coin 
    {
        void create(const sf::Vector2f& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/coin.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::COIN;

            Coin::Helper::setupAnimations(currentID);
            Coin::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupAnimations(EntityID id) noexcept
            {
                Manager::addComponent<Component::Animation>(id, BE_NULL::FALSE);
                static constexpr int animPos = 0;
                
                System::Animation::setFrames(id, animPos, {
                    sf::IntRect(0, 0, 10, 14),
                    sf::IntRect(16, 0, 10, 14),
                    sf::IntRect(32, 0, 10, 14),
                    sf::IntRect(48, 0, 10, 14),
                    sf::IntRect(3, 15, 3, 15),
                    sf::IntRect(17, 15, 7, 15),
                    sf::IntRect(35, 15, 4, 16),
                    sf::IntRect(53, 15, 1, 16)
                });

                System::Animation::setNextAnimationTimer(id, 100);
                System::Animation::setCurrentAnimation(id, animPos);
            }

            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id, BE_NULL::FALSE);
                Component::updates[id] = [](EntityID update_id) -> void {
                    System::Animation::play(update_id);
                };
            }
        } // namespace Helper
    } // namespace Coin

} // namespace Entity