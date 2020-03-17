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
            Fire::Helper::setupUpdateFunction(currentID);
        }

        void create(const sf::Vector2f& position) noexcept
        {
            EntityID currentID = Manager::create("assets/fire.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type     = Enum::Type::FIRE;
            Component::types[currentID] -> whatType = std::make_pair(IS_PLAYER::FALSE, std::nullopt);

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
            Fire::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id, BE_NULL::FALSE);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    System::Physics::start(update_id);

                    auto pairPlayer = std::get<PairIsPlayerID>(*Component::types[update_id].value().whatType);
                    if(bool(pairPlayer.first)) 
                    {
                        static sf::Clock clock;
                        auto playerLookingDirection = System::Movement::getLookingDirection(pairPlayer.second.value());
                        if(playerLookingDirection == Enum::Direction::RIGHT) 
                        {
                            System::Movement::moveRight(update_id, FIRE_SPEED);
                        } 
                        else if(playerLookingDirection == Enum::Direction::LEFT) 
                        {
                            System::Movement::moveLeft(update_id, FIRE_SPEED);
                        }

                        if(System::Physics::getOnGround(update_id)) 
                        {
                            System::Movement::jump(update_id, FIRE_HEIGHT, FORCE::FALSE);
                        }

                        if(System::Movement::getBlockedDirection(update_id) == Enum::Direction::LEFT ||
                            System::Movement::getBlockedDirection(update_id) == Enum::Direction::RIGHT ||
                            System::Movement::getBlockedDirection(update_id) == Enum::Direction::TOP)
                        {
                            System::Game::Helper::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                        } 
                        else if(sf::Time timer = clock.getElapsedTime();
                                timer >= sf::milliseconds(KILL_TIMER))
                        {
                            System::Game::Helper::removeID(update_id, WAIT_FOR_ANIM::FALSE);
                        }
                    } 
                    else {}
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
            Component::types[currentID] ->type     = Enum::Type::GOOMBA;

            Goomba::Helper::setupAnimation(currentID);

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
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
                    static bool moveLeft = true;

                    if(blockedDirection == Enum::Direction::RIGHT) {
                        moveLeft = false;
                    } 
                    else if(blockedDirection == Enum::Direction::LEFT) {
                        moveLeft = true;
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