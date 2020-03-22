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
        void create(const sf::Vector2f& position, EntityID player_id, Enum::Direction start_direction) noexcept
        {
            myAssert(start_direction == Enum::Direction::RIGHT &&
                     start_direction == Enum::Direction::LEFT, 
                     "Start Direction of Fire is wrong!");

            EntityID currentID = Manager::create("assets/fire.png");

            Enemy::Helper::setupBaseType(currentID, position, Enum::Type::FIRE);

            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);
            Manager::addComponent<Component::UpdateFunction>(currentID);
            Manager::addComponent<Component::GlobalVariables>(currentID);

            bool convert_direction = start_direction == Enum::Direction::RIGHT ? true : false;
            System::GlobalVariables::addAny(currentID, convert_direction); // index 0 - Direction

            Component::updates[currentID] = [](EntityID update_id) -> void
            {
                System::Physics::start(update_id);
                Fire::Helper::checkMovement(update_id);
                Fire::Helper::checkDeath(update_id);
            };
        }

        namespace Helper
        {
            void checkMovement(EntityID id) noexcept
            {
                const auto rightLookingDirection = System::GlobalVariables::getLastAny<bool>(id);
                if(rightLookingDirection) {
                    System::Movement::moveRight(id, FIRE_SPEED);
                } 
                else {
                    System::Movement::moveLeft(id, FIRE_SPEED);
                }

                // jump everytime touching the floor to make the bounce effect
                if(System::Physics::getOnGround(id)) {
                    System::Movement::jump(id, FIRE_HEIGHT, FORCE::FALSE);
                }
            }

            void checkDeath(EntityID id) noexcept
            {
                // if touching anything else than the ground, kill it
                const auto& clock = System::GlobalVariables::getClock(id);
                if(System::Movement::getBlockedDirection(id) == Enum::Direction::LEFT   ||
                   System::Movement::getBlockedDirection(id) == Enum::Direction::RIGHT  ||
                   System::Movement::getBlockedDirection(id) == Enum::Direction::BOTTOM)
                {
                    System::Game::removeID(id, WAIT_FOR_ANIM::FALSE);
                } 

                // timer to kill
                else if(sf::Time timer = clock.getElapsedTime();
                        timer >= sf::milliseconds(FIRE_KILL_TIMER))
                {
                    System::Game::removeID(id, WAIT_FOR_ANIM::FALSE);
                }
            }
        } // namespace Helper
    } // namespace Fire

    namespace Goomba
    {
        void create(const sf::Vector2f& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/goomba.png");

            Goomba::Helper::setupAnimation(currentID);
            Enemy::Helper::setupBaseType(currentID, position, Enum::Type::GOOMBA);
            System::GlobalVariables::addAny(currentID, true); // index 0 - moveLeft

            Component::updates[currentID] = [](EntityID update_id) -> void
            {
                System::Physics::start(update_id);
                System::Animation::play(update_id);

                Enemy::Helper::standardMovement(update_id);
            };
        }

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept
            {
                System::Animation::setFrames(id, int(Enum::Animation::WALK), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 16, 16)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(16, 0, 32, 16))
                });

                System::Animation::setFrames(id, int(Enum::Animation::DEAD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(32, 8, 48, 16)));

                System::Animation::setCurrentAnimation(id, int(Enum::Animation::WALK));
            }
        } // namespace Helper
    } // namespace Goomba

    namespace Spiny
    {
        void create(const sf::Vector2f& position) noexcept
        {
            EntityID currentID = Manager::create("assets/spiny.png");

            Spiny::Helper::setupAnimation(currentID);
            Enemy::Helper::setupBaseType(currentID, position, Enum::Type::SPINY);
            System::GlobalVariables::addAny(currentID, true); // index 0 - moveLeft

            Component::updates[currentID] = [](EntityID update_id) -> void
            {
                System::Physics::start(update_id);
                System::Animation::play(update_id);

                Enemy::Helper::standardMovement(update_id);
                Spiny::Helper::checkCorrectAnimation(update_id);
            };
        }

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept
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

            void checkCorrectAnimation(EntityID id) noexcept
            {
                const Enum::State state = System::Base::getState(id);
                if(state == Enum::State::ALIVE)
                {
                    const bool moveLeft = System::GlobalVariables::getLastAny<bool>(id);
                    if(moveLeft) {
                        System::Animation::setCurrentAnimation(id, int(Enum::Animation::WALK_LEFT));
                    } 
                    else  {
                        System::Animation::setCurrentAnimation(id, int(Enum::Animation::WALK_RIGHT));
                    }
                }
            }
        } // namespace Helper
    } // namespace Spiny

    namespace Helper
    {
        void setupBaseType(EntityID id, const sf::Vector2f& position, Enum::Type enemy_type) noexcept
        {
            System::Base::getSprite(id).setPosition(position);
            System::Base::setState(id, Enum::State::ALIVE);
            System::Type::setType(id, enemy_type);
        }

        void setupComponents(EntityID id) noexcept
        {
            Manager::addComponent<Component::Animation>(id);
            Manager::addComponent<Component::Movement>(id);
            Manager::addComponent<Component::Physics>(id);
            Manager::addComponent<Component::UpdateFunction>(id);
            Manager::addComponent<Component::GlobalVariables>(id);
        }

        // this is just a simple movement
        // walking to left / right and when touching some wall
        // change to the opposite direction
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