#include "player.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"
#include "../engine/helpers/functions.hpp"
#include "../engine/helpers/values.hpp"
#include "../engine/components.hpp"
#include "enemies.hpp"

namespace Entity 
{
    namespace Player 
    {
        void create(const sf::Vector2f& position, Enum::Mature maturity) noexcept
        {
            EntityID currentID = Manager::create("assets/mario.png");

            Component::bases[currentID].sprite.setPosition(position);
            Component::types[currentID].type     = Enum::Type::MARIO;
            Component::types[currentID].whatType = maturity;

            Player::Helper::setupAnimation(currentID);
            Manager::addComponent<Component::Movement>(currentID);
            Manager::addComponent<Component::Physics>(currentID);
            Manager::addComponent<Component::GlobalVariables>(currentID);
            Player::Helper::setupUpdateFunction(currentID);
        }

        namespace Helper
        {
            void setupAnimation(EntityID id) noexcept
            {
                Manager::addComponent<Component::Animation>(id);

                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::CHILD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(133, 99, 145, 114)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::CHILD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(83, 99, 95, 114)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::CHILD), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::CHILD), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(148, 99, 160, 114)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(163, 98, 177, 114))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::CHILD), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::CHILD), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(68, 99, 80, 114)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(50, 98, 65, 114))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_RIGHT, Enum::Mature::CHILD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(180, 98, 196, 114)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_LEFT, Enum::Mature::CHILD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(32, 98, 48, 114)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(116, 3, 130, 34)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(100, 3, 114, 33)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::TEENAGE), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(136, 4, 152, 34)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(156, 5, 172, 34))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::TEENAGE), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(78, 4, 94, 34)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(58, 5, 74, 34))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::TEENAGE), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(136, 4, 152, 34)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(156, 5, 172, 34))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::TEENAGE), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(78, 4, 94, 34)),
                    System::Animation::Helper::extractTextureRect(sf::IntRect(58, 5, 74, 34))
                });

                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_RIGHT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(175, 4, 191, 34)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_LEFT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(39, 4, 55, 34)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::CROUCH_RIGHT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(194, 12, 210, 34)));

                System::Animation::setFrames(id, sum<int>(Enum::Animation::CROUCH_LEFT, Enum::Mature::TEENAGE), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(20, 12, 36, 34)));

                auto& adultIdleRight = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_RIGHT, Enum::Mature::ADULT), adultIdleRight);

                auto& adultIdleLeft = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::IDLE_LEFT, Enum::Mature::ADULT), adultIdleLeft);

                auto& adultWalkRight = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_RIGHT, Enum::Mature::ADULT), adultWalkRight);

                auto& adultWalkLeft = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::WALK_LEFT, Enum::Mature::ADULT), adultWalkLeft);

                auto& adultRunRight = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_RIGHT, Enum::Mature::ADULT), adultRunRight);

                auto& adultRunLeft = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::RUN_LEFT, Enum::Mature::ADULT), adultRunLeft);

                auto& adultJumpRight = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::JUMP_RIGHT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_RIGHT, Enum::Mature::ADULT), adultJumpRight);

                auto& adultJumpLeft = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::JUMP_LEFT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::JUMP_LEFT, Enum::Mature::ADULT), adultJumpLeft);

                auto& adultCrouchRight = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::CROUCH_RIGHT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::CROUCH_RIGHT, Enum::Mature::ADULT), adultCrouchRight);

                auto& adultCrouchLeft = throw_if_null(System::Animation::getFrames(id, sum<int>(Enum::Animation::CROUCH_LEFT, Enum::Mature::TEENAGE)));
                System::Animation::setFrames(id, sum<int>(Enum::Animation::CROUCH_LEFT, Enum::Mature::ADULT), adultCrouchLeft);

                System::Animation::setAllowPlay(id, true);
            }

            void setupUpdateFunction(EntityID id) noexcept
            {
                Manager::addComponent<Component::UpdateFunction>(id);
                Component::updates[id] = [](EntityID update_id) -> void
                {
                    auto& base = Component::bases[update_id];
                    static const sf::Texture defaultTexture = *base.sprite.getTexture();
                    static const sf::Texture whiteTexture   = changeMarioRednessColor(*base.sprite.getTexture(), sf::Color(255, 250, 250));

                    System::Movement::setMoving(update_id, false);

                    System::Physics::start(update_id);

                    // get the maturity from the optional variant
                    const auto maturity = std::get<Enum::Mature>(Component::types[update_id].whatType.value());
                    const auto lookingDirection = System::Movement::getLookingDirection(update_id);

                    Player::Helper::startMovement(update_id, maturity, lookingDirection);

                    if(maturity == Enum::Mature::ADULT) {
                        base.sprite.setTexture(whiteTexture);
                    } 
                    else {
                        base.sprite.setTexture(defaultTexture);
                    }

                    System::Animation::play(update_id);
                };
            }

            void startMovement(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction)
            {
                float speed = Player::Helper::checkSpeed();

                Player::Helper::checkJump(id, maturity, looking_direction);
                Player::Helper::checkShooting(id, maturity);

                if(Player::Helper::checkCrouch(id, maturity, looking_direction))                    {}
                else if(Player::Helper::checkMovementRight(id, speed, maturity, looking_direction)) {}
                else if(Player::Helper::checkMovementLeft(id, speed, maturity, looking_direction))  {}
                else Player::Helper::IDLE(id, maturity, looking_direction);
            }
            
            void checkJump(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction) noexcept
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
                {
                    System::Movement::jump(id, PLAYER_JUMP, FORCE::FALSE);
                    if(System::Movement::getJumping(id) == true)
                    {
                        switch(looking_direction)
                        {
                            case Enum::Direction::LEFT:
                                System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::JUMP_LEFT, maturity));
                            break; 

                            case Enum::Direction::RIGHT:
                                System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::JUMP_RIGHT, maturity));
                            break;

                            // To make the compiler happy
                            default: 
                            break;
                        }
                    }
                }
            }

            void checkShooting(EntityID id, Enum::Mature maturity)
            {
                if(maturity == Enum::Mature::ADULT)
                {
                    auto& clock = System::GlobalVariables::getClock(id);
                    if(sf::Time timer = clock.getElapsedTime();
                        timer >= sf::milliseconds(PLAYER_FIRE))
                    {
                        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) 
                        {
                            auto& base = Component::bases[id];
                            Enemy::Fire::create(base.sprite.getPosition(), id);
                            clock.restart();
                        }
                    }
                }
            }

            bool checkCrouch(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction) noexcept
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                {
                    if(maturity != Enum::Mature::CHILD)
                    {
                        switch(looking_direction)
                        {
                            case Enum::Direction::LEFT:
                                System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::CROUCH_LEFT, maturity));
                                return true;
                            break; 

                            case Enum::Direction::RIGHT:
                                System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::CROUCH_RIGHT, maturity));
                                return true;
                            break;

                            // To make the compiler happy
                            default: 
                            break;
                        }
                    }
                }

                return false;
            }

            bool checkMovementRight(EntityID id, float speed, Enum::Mature maturity, Enum::Direction looking_direction) noexcept
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && 
                   System::Movement::getBlockedDirection(id) != Enum::Direction::LEFT) /* make sure the player is not touching the 
                                                                                        the left side of the object to prevent overlapping */ 
                {
                    // Don't let the player to be inside an object after crouching
                    if(Player::Helper::checkCrouching(id)){
                        System::Movement::jump(id, 30, FORCE::FALSE);
                    }

                    System::Movement::setLookingDirection(id, Enum::Direction::RIGHT);
                    System::Movement::moveRight(id, speed);
                    
                    if(Player::Helper::checkPlayerRunning(speed)) 
                    {
                        System::Movement::setRunning(id, true);
                        if(System::Physics::getOnGround(id)) 
                        {
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::RUN_RIGHT, maturity));
                            System::Animation::setNextAnimationTimer(id, 100);
                        }
                    } 
                    else 
                    {
                        System::Movement::setRunning(id, false);
                        if(System::Physics::getOnGround(id)) 
                        {
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::WALK_RIGHT, maturity));
                            System::Animation::setNextAnimationTimer(id, 150);
                        }
                    }

                    return true;
                }

                return false;
            }

            bool checkMovementLeft(EntityID id, float speed, Enum::Mature maturity, Enum::Direction looking_direction) noexcept
            {
                if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && 
                    System::Movement::getBlockedDirection(id) != Enum::Direction::RIGHT) /* make sure the player is not touching the 
                                                                                            the right side of the object to prevent overlapping*/ 
                {
                    // Don't let the player to be inside an object after crouching
                    if(Player::Helper::checkCrouching(id)){
                        System::Movement::jump(id, 30, FORCE::FALSE);
                    }

                    System::Movement::setLookingDirection(id, Enum::Direction::LEFT);
                    System::Movement::moveLeft(id, speed);
                    
                    if(Player::Helper::checkPlayerRunning(speed)) 
                    {
                        System::Movement::setRunning(id, true);
                        if(System::Physics::getOnGround(id)) 
                        {
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::RUN_LEFT, maturity));
                            System::Animation::setNextAnimationTimer(id, 100);
                        }
                    } 
                    else 
                    {
                        System::Movement::setRunning(id, false);
                        if(System::Physics::getOnGround(id)) 
                        {
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::WALK_LEFT, maturity));
                            System::Animation::setNextAnimationTimer(id, 150);
                        }
                    }

                    return true;
                }

                return false;
            }

            void IDLE(EntityID id, Enum::Mature maturity, Enum::Direction looking_direction)
            {
                if(System::Physics::getOnGround(id))
                {
                    // Don't let the player to be inside an object after crouching
                    if(Player::Helper::checkCrouching(id)){
                        System::Movement::jump(id, 30, FORCE::FALSE);
                    }

                    // IDLE
                    switch(looking_direction)
                    {
                        case Enum::Direction::LEFT:
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::IDLE_LEFT, maturity));
                        break; 

                        case Enum::Direction::RIGHT:
                            System::Animation::setCurrentAnimation(id, sum<int>(Enum::Animation::IDLE_RIGHT, maturity));
                        break;

                        // To make the compiler happy
                        default: 
                        break;
                    }
                }
            }

            float checkSpeed() noexcept
            {
                // if shift is been pressed change the `speed` to running
                return sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ? SHIFTING_PLAYER_SPEED : PLAYER_SPEED;
            }

            bool checkPlayerRunning(float speed) noexcept
            {
                return speed != PLAYER_SPEED;
            }

            bool checkCrouching(EntityID id) noexcept
            {
                int currentAnim = System::Animation::getCurrentAnimation(id);
                if(currentAnim == sum<int>(Enum::Animation::CROUCH_RIGHT, Enum::Mature::TEENAGE) ||
                   currentAnim == sum<int>(Enum::Animation::CROUCH_LEFT , Enum::Mature::TEENAGE) ||
                   currentAnim == sum<int>(Enum::Animation::CROUCH_RIGHT, Enum::Mature::ADULT)   ||
                   currentAnim == sum<int>(Enum::Animation::CROUCH_LEFT , Enum::Mature::ADULT))
                {
                    return true;
                }

                return false;
            }

            sf::Texture changeMarioRednessColor(sf::Texture texture, const sf::Color& color) noexcept
            {
                static const std::initializer_list<sf::Color> marioRedColors { sf::Color(160, 0, 0),
                                                                               sf::Color(200, 0, 24),
                                                                               sf::Color(248, 32, 56) };
                sf::Image img = texture.copyToImage();
                for(unsigned int y = 0; y <= img.getSize().y; y++)
                {
                    for(unsigned int x = 0; x <= img.getSize().x; x++)
                    {
                        auto pixel = img.getPixel(x, y);

                        for(auto& c : marioRedColors)
                        {
                            if(pixel.r == c.r &&
                               pixel.g == c.g &&
                               pixel.b == c.b)
                            {
                                img.setPixel(x, y, color);
                            }
                        }
                    }        
                }

                texture.loadFromImage(img);
                return texture;
            }
        } // namespace Helper
    } // namespace Player
} // namespace Entity