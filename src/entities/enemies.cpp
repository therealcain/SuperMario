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

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type     = Enum::Type::FIRE;

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

    namespace Goomba 
    {
        void create(const sf::Vector2f& position) noexcept
        {  
            EntityID currentID = Manager::create("assets/goomba.png");

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type = Enum::Type::GOOMBA;

            Goomba::Helper::setupAnimation(currentID);

            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);
            Manager::addComponent<Component::GlobalVariables>(currentID);
            System::GlobalVariables::addAny(currentID, true); // 0

            Goomba::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept
            {
                Manager::addComponent<Component::Animation>(id);
                System::Animation::setFrames(id, int(Enum::Animation::WALK), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 16, 16)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(16, 0, 32, 16))
                });

                System::Animation::setFrames(id, int(Enum::Animation::DEAD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(32, 8, 48, 16)));

                System::Animation::setCurrentAnimation(id, int(Enum::Animation::WALK));
            }

            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void 
                {
                    System::Physics::start(update_id);

                    const auto blockedDirection = System::Movement::getBlockedDirection(update_id);
                    const bool moveLeft = System::GlobalVariables::getLastAny<bool>(update_id);

                    if(blockedDirection == Enum::Direction::RIGHT) {
                        System::GlobalVariables::setLastAny(update_id, false);
                    } 
                    else if(blockedDirection == Enum::Direction::LEFT) {
                        System::GlobalVariables::setLastAny(update_id, true);
                    }

                    if(moveLeft) {
                        System::Movement::moveLeft(update_id, ENEMY_SPEED);
                    } 
                    else {
                        System::Movement::moveRight(update_id, ENEMY_SPEED);
                    }

                    System::Animation::play(update_id);
                };
            }
        } // namespace Helper
    } // namespace Goomba
} // namespace Enemy