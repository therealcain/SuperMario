#include "enemies.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"
#include "../engine/helpers/functions.hpp"
#include "../engine/helpers/values.hpp"
#include "../engine/components.hpp"

namespace Enemy
{
    // ---------------------------------------------------------- //
    // -------------------------- Fire -------------------------- //
    // ---------------------------------------------------------- //
    namespace Fire
    {
        void create(const sf::Vector2f& position, EntityID player_id) noexcept
        {
            EntityID currentID = Manager::create("assets/fire.png");

            System::Base::getSprite(currentID).setPosition(position);
            System::Type::setType(currentID, Enum::Type::FIRE);

            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);
            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID, player_id); // index 0
            System::GlobalVariables::addAny(currentID); // index 1

            Fire::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    System::Physics::start(update_id);

                    // get where the player is looking at and set it once in globalVariables
                    EntityID playerID = System::GlobalVariables::getAny<EntityID>(update_id, 0);
                    const auto playerLookingDirection = System::Movement::getLookingDirection(playerID);
                    if(playerLookingDirection == Enum::Direction::RIGHT) {
                        System::GlobalVariables::setAnyOnce(update_id, true, 1);
                    } 
                    else if(playerLookingDirection == Enum::Direction::LEFT) {
                        System::GlobalVariables::setAnyOnce(update_id, false, 1);
                    }

                    // get where the player was looking at and move to the same direction
                    const auto fireRightLookingDirection = System::GlobalVariables::getAny<bool>(update_id, 1);
                    if(fireRightLookingDirection) {
                        System::Movement::moveRight(update_id, FIRE_SPEED);
                    } 
                    else {
                        System::Movement::moveLeft(update_id, FIRE_SPEED);
                    }

                    // jump everytime touching the floor to make the bounce effect
                    if(System::Physics::getOnGround(update_id)) {
                        System::Movement::jump(update_id, FIRE_HEIGHT, FORCE::FALSE);
                    }

                    // if touching anything else than the ground, kill it
                    const auto& clock = System::GlobalVariables::getClock(update_id);
                    if(System::Movement::getBlockedDirection(update_id) == Enum::Direction::LEFT   ||
                        System::Movement::getBlockedDirection(update_id) == Enum::Direction::RIGHT ||
                        System::Movement::getBlockedDirection(update_id) == Enum::Direction::BOTTOM)
                    {
                        System::Game::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                    } 

                    // timer to kill
                    else if(sf::Time timer = clock.getElapsedTime();
                            timer >= sf::milliseconds(FIRE_KILL_TIMER))
                    {
                        System::Game::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                    }
                };
            }
        } // namespace Helper
    } // namespace Fire

    void create(const sf::Vector2f& position, Enum::Type enemy_type) noexcept
    {
        Helper::checkErrors(enemy_type);

        EntityID currentID;
        switch (enemy_type)
        {
        case Enum::Type::GOOMBA:
            currentID = Manager::create("assets/goomba.png");
        break;
        
        case Enum::Type::SPINY:
            currentID = Manager::create("assets/spiny.png");
        break;

        default:
        break;
        }

        System::Base::getSprite(currentID).setPosition(position);
        System::Base::setState(currentID, Enum::State::ALIVE);
        System::Type::setType(currentID, enemy_type);

        Manager::addComponent<Component::Animation>(currentID);
        Helper::setupAnimation(currentID, enemy_type);

        Manager::addComponent<Component::Movement>(currentID);
        Manager::addComponent<Component::Physics>(currentID);
        Manager::addComponent<Component::UpdateFunction>(currentID);

        Manager::addComponent<Component::GlobalVariables>(currentID);
        System::GlobalVariables::addAny(currentID, true); // index 0 - moveLeft

        Helper::setupUpdateFunction(currentID);
    }

    namespace Helper
    {
        void checkErrors(Enum::Type enemy_type)
        {
            if(enemy_type != Enum::Type::GOOMBA &&
               enemy_type != Enum::Type::SPINY)
            {
                throw std::runtime_error("Enemy type is not allowed!");
            }
        }

        void setupAnimation(EntityID id, Enum::Type type) noexcept
        {
            Manager::addComponent<Component::Animation>(id);
            switch (type)
            {
            case Enum::Type::GOOMBA:
            {
                System::Animation::setFrames(id, int(Enum::Animation::WALK), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 16, 16)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(16, 0, 32, 16))
                });

                System::Animation::setFrames(id, int(Enum::Animation::DEAD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(32, 8, 48, 16)));

                System::Animation::setCurrentAnimation(id, int(Enum::Animation::WALK));
            }
            break;
            
            case Enum::Type::SPINY:
            {
                System::Animation::setFrames(id, int(Enum::Animation::WALK_RIGHT), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(47, 0, 63, 16)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(63, 0, 79, 16))
                });

                System::Animation::setFrames(id, int(Enum::Animation::WALK_LEFT), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(31, 0, 47, 16)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(15, 0, 31, 16))
                });

                System::Animation::setFrames(id, int(Enum::Animation::DEAD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 14, 16)));
            }
            break;

            default:
            break;
            }
        }

        void setupUpdateFunction(EntityID id) noexcept
        {
            Component::updates[id] = [](EntityID update_id) -> void
            {
                System::Physics::start(update_id);

                auto type = System::Type::getType(update_id);
                switch(type)
                {
                    case Enum::Type::GOOMBA:
                        standardMovement(update_id);
                    break;

                    case Enum::Type::SPINY:
                    {
                        standardMovement(update_id);
                        const Enum::State state = System::Base::getState(update_id);
                        if(state == Enum::State::ALIVE)
                        {
                            const bool moveLeft = System::GlobalVariables::getLastAny<bool>(update_id);
                            if(moveLeft) {
                                System::Animation::setCurrentAnimation(update_id, int(Enum::Animation::WALK_LEFT));
                            } 
                            else  {
                                System::Animation::setCurrentAnimation(update_id, int(Enum::Animation::WALK_RIGHT));
                            }
                        }
                        
                    }
                    break;

                    default:
                    break;
                }

                System::Animation::play(update_id);
            };
        }

        void standardMovement(EntityID id) noexcept
        {
            const Enum::State state = System::Base::getState(id);

            if(state == Enum::State::ALIVE)
            {
                const auto blockedDirection = System::Movement::getBlockedDirection(id);
                const bool moveLeft         = System::GlobalVariables::getLastAny<bool>(id);

                if(blockedDirection == Enum::Direction::RIGHT) {
                    System::GlobalVariables::setLastAny(id, false);
                } 
                else if(blockedDirection == Enum::Direction::LEFT) {
                    System::GlobalVariables::setLastAny(id, true);
                }

                if(moveLeft) {
                    System::Movement::moveLeft(id, ENEMY_SPEED);
                } 
                else {
                    System::Movement::moveRight(id, ENEMY_SPEED);
                }
            }
        }
    } // Helper
} // namespace Enemy