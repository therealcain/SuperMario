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

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type     = Enum::Type::BLOCK;
            Component::types[currentID].whatType = std::make_pair(block_type, 
                                                                type.has_value() ? type.value() : Enum::Type::NONE);

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
                            type.value() != Enum::Type::STAR     &&
                            type.value() != Enum::Type::ONEUP)
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

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type = Enum::Type::CLOUD;
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

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type = Enum::Type::COIN;

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

    // namespace Mushroom
    // {
    //     void create(const sf::Vector2f& position, GET_FROM get_from) noexcept
    //     {
    //         EntityID currentID = Manager::create("assets/mushroom.png");

    //         Component::bases[currentID].sprite.setPosition(position);
    //         Component::types[currentID].type = Enum::Type::MUSHROOM;
    //         Component::types[currentID].whatType = bool(get_from);

    //         Manager::addComponent<Component::Movement>(currentID);
    //         Manager::addComponent<Component::Physics>(currentID);
    //         Manager::addComponent<Component::GlobalVariables>(currentID);

    //         Coin::Helper::setupUpdateFunction(currentID);
    //     }

    //     namespace Helper
    //     {
    //         void setupUpdateFunction(EntityID id) noexcept
    //         {
    //             Manager::addComponent<Component::UpdateFunction>(id);
    //             Component::updates[id] = [](EntityID update_id) -> void
    //             {
    //                 bool getFrom = std::get<bool>(*Component::types[update_id].value().whatType);
    //                 if(getFrom) {
    //                     System::GlobalVariables::setGetOutOnce(update_id, true);
    //                 }

    //                 if(System::GlobalVariables::getGetFrom(update_id)) {
    //                     System::Movement::jump(update_id, 30, FORCE::TRUE);
    //                 }       
    //             };
    //         }
    //     } // namespace Helper

    // } // namespace Mushroom

} // namespace Entity