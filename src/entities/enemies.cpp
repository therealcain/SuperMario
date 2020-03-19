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

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type     = Enum::Type::FIRE;
            Component::types[currentID] -> whatType = std::make_pair(IS_PLAYER::TRUE, player_id);

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Global>(currentID, BE_NULL::FALSE);
            Fire::Helper::setupUpdateFunction(currentID);
        }

        // void create(const sf::Vector2f& position) noexcept
        // {
        //     EntityID currentID = Manager::create("assets/fire.png");

        //     Component::bases[currentID] -> sprite.setPosition(position);
        //     Component::types[currentID] -> type     = Enum::Type::FIRE;
        //     Component::types[currentID] -> whatType = std::make_pair(IS_PLAYER::FALSE, std::nullopt);

        //     Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
        //     Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
        //     Manager::addComponent<Component::Global>(currentID, BE_NULL::FALSE);
        //     Fire::Helper::setupUpdateFunction(currentID);
        // }

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id, BE_NULL::FALSE);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    System::Physics::start(update_id);

                    auto pairPlayer = std::get<PairIsPlayerID>(*Component::types[update_id].value().whatType);
                    bool isPlayer = bool(pairPlayer.first);
                    if(isPlayer) 
                    {
                        EntityID playerID = pairPlayer.second.value();
                        const auto& clock = System::Global::getClock(update_id);

                        // get where the player is looking at and set it once in globals
                        const auto playerLookingDirection = System::Movement::getLookingDirection(playerID);
                        if(playerLookingDirection == Enum::Direction::RIGHT) {
                            System::Global::setLeftLookingDirectionOnce(update_id, true);
                        } 
                        else if(playerLookingDirection == Enum::Direction::LEFT) {
                            System::Global::setLeftLookingDirectionOnce(update_id, false);
                        }

                        // get where the player was looking at and move to the same direction
                        const auto fireRightLookingDirection = throw_if_null(System::Global::getLeftLookingDirection(update_id));
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
                        if(System::Movement::getBlockedDirection(update_id) == Enum::Direction::LEFT   ||
                            System::Movement::getBlockedDirection(update_id) == Enum::Direction::RIGHT ||
                            System::Movement::getBlockedDirection(update_id) == Enum::Direction::BOTTOM)
                        {
                            System::Game::Helper::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                        } 

                        // timer to kill
                        else if(sf::Time timer = clock.getElapsedTime();
                                timer >= sf::milliseconds(FIRE_KILL_TIMER))
                        {
                            System::Game::Helper::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                        }
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

            Component::bases[currentID] ->sprite.setPosition(position);
            Component::types[currentID] ->type = Enum::Type::GOOMBA;

            Goomba::Helper::setupAnimation(currentID);

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Global>(currentID, BE_NULL::FALSE);
            Goomba::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept
            {
                Manager::addComponent<Component::Animation>(id, BE_NULL::FALSE);
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
                Manager::addComponent<Component::UpdateFunction>(id, BE_NULL::FALSE);
                Component::updates[id] = [](EntityID update_id) -> void 
                {
                    System::Physics::start(update_id);

                    const auto blockedDirection = System::Movement::getBlockedDirection(update_id);
                    const bool moveLeft = System::Global::getMoveLeft(update_id);

                    if(blockedDirection == Enum::Direction::RIGHT) {
                        System::Global::setMoveLeft(update_id, false);
                    } 
                    else if(blockedDirection == Enum::Direction::LEFT) {
                        System::Global::setMoveLeft(update_id, true);
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