#include "entities.hpp"
#include "../engine/system.hpp"

#include <iostream>
#include <cassert>

namespace Entity
{
    // ---------------------------------------------------------- //
    // -------------------------- BLOCK ------------------------- //
    // ---------------------------------------------------------- //
    namespace Block
    {
        void create(const sf::Vector2f& position, Enum::Block block_type, std::optional<Enum::Type> type) noexcept
        {
            Block::Helper::errorCheck(block_type, type);

            EntityID currentID = Manager::create("assets/block.png");
            
            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::BLOCK);
            System::Type::setWhatType(currentID, std::make_pair(block_type, 
                                                        type.has_value() ? type.value() : Enum::Type::NONE));

            Manager::addComponent<Component::Physics>(currentID);
            System::Physics::setRigidbody(currentID, true);

            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID); // index 0 - miscellaneous started

            Block::Helper::setupAnimations(currentID, block_type);
            Block::Helper::setupUpdateFunction(currentID);
        }   

        namespace Helper
        {
            void errorCheck(Enum::Block block_type, std::optional<Enum::Type> type)
            {
                if(block_type == Enum::Block::QUESTION_MARK) 
                {
                    if(not type.has_value()) {
                        throw std::runtime_error("Type assignation is wrong! hint:\n"
                                                "you must assign to question_mark block something!\n");
                    } 
                    else if(type.value() != Enum::Type::MUSHROOM &&
                            type.value() != Enum::Type::FLOWER   &&
                            type.value() != Enum::Type::STAR)
                    {
                        throw std::runtime_error("Type assignation is wrong! hint:\n"
                                                "you assigned something unaccepted to question_mark block!\n");
                    }
                } 
                else 
                {
                    if(type.has_value()) {
                        throw std::runtime_error("Type assignation is wrong! hint:\n"
                                                "you can't assign type to other blocks than question_mark block!\n");
                    }
                }
            }

            void setupAnimations(EntityID id, Enum::Block type) noexcept
            {
                Manager::addComponent<Component::Animation>(id);
                switch(type) {
                    case Enum::Block::EMPTY:
                        System::Animation::setFrames(id, int(Enum::Block::EMPTY), sf::IntRect(64, 0, 16, 16));
                    break;

                    case Enum::Block::BRICK:
                        System::Animation::setFrames(id, int(Enum::Block::BRICK), {
                            System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 16, 16)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(16, 0, 32, 16)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(32, 0, 48, 16)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(48, 0, 64, 16))
                        });
                    break;

                    case Enum::Block::QUESTION_MARK:
                        System::Animation::setFrames(id, int(Enum::Block::QUESTION_MARK), {
                            System::Animation::Helper::extractTextureRect(sf::IntRect(0, 16, 16, 32)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(16, 16, 32, 32)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(32, 16, 48, 32)),
                            System::Animation::Helper::extractTextureRect(sf::IntRect(48, 0, 64, 16))
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
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void {
                    System::Animation::play(update_id);

                    if(System::Animation::getStarted(update_id)) {
                        System::GlobalVariables::setAnyOnce(update_id, true, 0);
                    
                        if(System::GlobalVariables::getLastAny<bool>(update_id)) {
                            System::GlobalVariables::setAny(update_id, false, 0);

                            auto& pair = System::Type::getBlockPair(update_id);
                            switch(pair.second)
                            {
                                case Enum::Type::MUSHROOM:
                                Entity::Mushroom::create(System::Base::getSprite(update_id).getPosition());
                                break;

                                case Enum::Type::FLOWER:
                                Entity::Flower::create(System::Base::getSprite(update_id).getPosition());
                                break;

                                // to make the compiler happy
                                default:
                                break;
                            }
                        }
                    }
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

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::CLOUD);
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

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::COIN);

            Coin::Helper::setupAnimations(currentID);
            Coin::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupAnimations(EntityID id) noexcept
            {
                Manager::addComponent<Component::Animation>(id);
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
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void {
                    System::Animation::play(update_id);
                };
            }
        } // namespace Helper
    } // namespace Coin

    // ---------------------------------------------------------- //
    // -------------------------- MUSHROOM ---------------------- //
    // ---------------------------------------------------------- //
    namespace Mushroom
    {
        void create(const sf::Vector2f& position) noexcept
        {
            EntityID currentID = Manager::create("assets/mushroom.png");

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::MUSHROOM);

            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);

            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID, true); // index 0 - Jumped
            System::GlobalVariables::addAny(currentID, false); // index 1 - moveLeft

            System::Physics::setSpeed(currentID, POP_OUT_MUSHROOM_SPEED);

            Mushroom::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    System::Physics::start(update_id);

                    Helper::jumpOutOnce(update_id);
                    Helper::checkBlockedDirections(update_id);
                    Helper::movement(update_id);
                };
            }

            void jumpOutOnce(EntityID id) noexcept
            {
                if(System::GlobalVariables::getAny<bool>(id, 0)) 
                {
                    System::Movement::jump(id, 100, FORCE::TRUE);
                    System::GlobalVariables::setAny(id, false, 0);
                }
            }

            void checkBlockedDirections(EntityID id) noexcept
            {
                const auto blockedDirection = System::Movement::getBlockedDirection(id);
                if(blockedDirection == Enum::Direction::RIGHT) {
                    System::GlobalVariables::setAny(id, false, 1);
                } 
                else if(blockedDirection == Enum::Direction::LEFT) {
                    System::GlobalVariables::setAny(id, true, 1);
                }
            }

            void movement(EntityID id) noexcept
            {
                const bool moveLeft = System::GlobalVariables::getAny<bool>(id, 1);
                if(moveLeft) {
                    System::Movement::moveLeft(id, ENEMY_SPEED);
                } 
                else {
                    System::Movement::moveRight(id, ENEMY_SPEED);
                }
            }
        } // namespace Helper
    } // namespace Mushroom

    // ---------------------------------------------------------- //
    // -------------------------- FLOWER ------------------------ //
    // ---------------------------------------------------------- //
    namespace Flower
    {
        void create(const sf::Vector2f& position) noexcept
        {
            EntityID currentID = Manager::create("assets/flower.png");

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::FLOWER);

            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);
            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID, true); // index 0 - Jumped

            System::Physics::setSpeed(currentID, POP_OUT_FLOWER_SPEED);

            Flower::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    System::Physics::start(update_id);
                    Helper::jumpOutOnce(update_id);
                };
            }

            void jumpOutOnce(EntityID id) noexcept
            {
                if(System::GlobalVariables::getAny<bool>(id, 0)) 
                {
                    System::Movement::jump(id, 100, FORCE::TRUE);
                    System::GlobalVariables::setAny(id, false, 0);
                }
            }
        } // namespace Helper
    } // namespace Flower

    // ---------------------------------------------------------- //
    // -------------------------- PIPE -------------------------- //
    // ---------------------------------------------------------- // 
    namespace Pipe
    {
        void create(const sf::Vector2f& position, std::optional<sf::Vector2f> target_position) noexcept
        {
            EntityID currentID = Manager::create("assets/pipe.png");

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::PIPE);

            Manager::addComponent<Component::Physics>(currentID);
            System::Physics::setRigidbody(currentID, true);

            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID, false); // index 0 - have target_position or not
            if(target_position.has_value())
            {
                System::GlobalVariables::setAny(currentID, true, 0); // change target_position to true
                System::GlobalVariables::addAny(currentID, target_position->x); // x pos
                System::GlobalVariables::addAny(currentID, target_position->y); // y pos
            }
        }
    } // namespace Pipe

} // namespace Entity