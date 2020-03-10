#include "player.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"
#include "../engine/helpers/functions.hpp"
#include "../engine/helpers/values.hpp"
#include "../engine/components.hpp"

namespace Entity 
{
    namespace Player 
    {
        void create(sf::Vector2f&& position, Enum::Mature maturity) noexcept
        {
            EntityID currentID = Manager::create("assets/mario.png");

            Component::bases[currentID] ->sprite.setPosition(position);
            Component::types[currentID] ->type     = Enum::Type::MARIO;
            Component::types[currentID] ->whatType = maturity;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) 
            {   
                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(133, 99, 145, 114)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::CHILD), 
                    System::Animation::extractTextureRect(sf::IntRect(83, 99, 95, 114)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
                    System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::CHILD), {
                    System::Animation::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
			        System::Animation::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::TEENAGE), 
                    System::Animation::extractTextureRect(sf::IntRect(116, 3, 130, 34)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::TEENAGE), 
                    System::Animation::extractTextureRect(sf::IntRect(100, 3, 114, 33)));

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::TEENAGE), {
                    System::Animation::extractTextureRect(sf::IntRect(136, 4, 152, 34)),
                    System::Animation::extractTextureRect(sf::IntRect(156, 5, 172, 34))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::TEENAGE), {
                    System::Animation::extractTextureRect(sf::IntRect(78, 4, 94, 34)),
                    System::Animation::extractTextureRect(sf::IntRect(58, 5, 74, 34))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::TEENAGE), {
                    System::Animation::extractTextureRect(sf::IntRect(136, 4, 152, 34)),
                    System::Animation::extractTextureRect(sf::IntRect(156, 5, 172, 34))
                });

                System::Animation::setFrames(currentID, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::TEENAGE), {
                    System::Animation::extractTextureRect(sf::IntRect(78, 4, 94, 34)),
                    System::Animation::extractTextureRect(sf::IntRect(58, 5, 74, 34))
                });
            }

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) -> void
            {
                // just in case whatType doesn't respond well
                #ifdef ENABLE_DEBUG_MODE
                {
                    try{
                        [[maybe_unused]] const auto maturity = std::get<Enum::Mature>(Component::types[id] ->whatType.value());
                    }
                    catch(std::exception& e) {
                        Debug::print("ID: ", id, " - Failed to access maturity!");
                    }
                }
                #endif
            
                if(System::Physics::isPrepared(id)) {
                    System::Physics::start(id);
                }

                Helper::startMovement(id);
                System::Animation::play(id);
            };
        }

        namespace Helper
        {
            void startMovement(EntityID id)
            {
                if(System::Movement::isPrepared(id, WITH_PHYSICS::TRUE))
                {
                    // get the maturity from the optional variant
                    const auto maturity = std::get<Enum::Mature>(Component::types[id] ->whatType.value());
                    // if shift is been pressed change the `speed` to running
                    float speed = DEFAULT_SPEED;
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                        speed += 1;
                    }

                    // if(speed != DEFAULT_SPEED) = Running
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
                    {
                        System::Animation::setAllowPlay(id, ALLOW::TRUE);
                        System::Movement::moveRight(id, speed);
                        
                        if(speed != DEFAULT_SPEED && not System::Physics::isMidAir(id)) {
                            System::Movement::setRunning(id, RUNNING::TRUE);
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::RUN_RIGHT, maturity));
                            System::Animation::setNextAnimationTimer(id, 100);
                        } 
                        else if(speed != DEFAULT_SPEED && System::Physics::isMidAir(id)) {
                            System::Movement::setRunning(id, RUNNING::TRUE);
                        } 
                        else {
                            System::Movement::setRunning(id, RUNNING::FALSE);
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::WALK_RIGHT, maturity));
                            System::Animation::setNextAnimationTimer(id, 150);
                        }
                    } else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
                    {
                        System::Animation::setAllowPlay(id, ALLOW::TRUE);
                        System::Movement::moveLeft(id, speed);
                        
                        if(speed != DEFAULT_SPEED && not System::Physics::isMidAir(id)) {
                            System::Movement::setRunning(id, RUNNING::TRUE);
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::RUN_LEFT, maturity));
                            System::Animation::setNextAnimationTimer(id, 100);
                        } 
                        else if(speed != DEFAULT_SPEED && System::Physics::isMidAir(id)) {
                            System::Movement::setRunning(id, RUNNING::TRUE);
                        } 
                        else {
                            System::Movement::setRunning(id, RUNNING::FALSE);
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::WALK_LEFT, maturity));
                            System::Animation::setNextAnimationTimer(id, 150);
                        }
                    } else 
                    {
                        if(System::Physics::isOnGround(id)) {
                            // IDLE
                            System::Animation::setAllowPlay(id, ALLOW::FALSE);
                            const auto lookingDirection = System::Movement::getLookingDirection(id);
                            switch(lookingDirection)
                            {
                                case Enum::Direction::LEFT:
                                    System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::IDLE_LEFT, maturity));
                                break; 

                                case Enum::Direction::RIGHT:
                                    System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::IDLE_RIGHT, maturity));
                                break;

                                case Enum::Direction::TOP:    [[fallthrough]];
                                case Enum::Direction::BOTTOM: [[fallthrough]];
                                case Enum::Direction::NONE:   [[fallthrough]];
                                default: break;
                            }
                        }
                    }
                }
            }
        } // namespace Helper
    } // namespace Player
} // namespace Entity