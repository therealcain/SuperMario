#include "system.hpp"
#include "helpers/values.hpp"
#include "helpers/functions.hpp"
#include "manager.hpp"

#include <exception>
#include <cmath>
#include <vector>

namespace System
{
    // ----------- Render ------------ //
    void Render::draw(EntityID id) noexcept 
    {
        if(Component::bases[id].has_value()) 
        {
            auto& base = Component::bases[id].value();
            m_window.draw(base.sprite);
        }
    }

    void Render::drawAll() noexcept 
    {
        for(size_t i = 0; i < Component::maxIndexes; i++) {
            Render::draw(i);
        }
    }

    // ----------- Game Loop ------------ //
    namespace Game
    {
        void update(EntityID id) noexcept 
        {
            if(Component::updates[id].has_value())
            {
                auto& update = Component::updates[id].value();
                update(id);
            }
        }

        void updateAll() noexcept 
        {
            for(size_t i = 0; i < Component::updates.size(); i++) {
                Game::update(i);
            }
        }
    } // namespace Game


    // ----------- Animation ------------ //
    namespace Animation 
    {
        bool isPrepared(EntityID id)
        {
            if(not Component::animations[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - animations'");
            }

            if(not Component::bases[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - bases'");
            }

            return SUCCESS;
        }

        void setFrames(EntityID id, int pos, const AnimationVector&& anims) noexcept 
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos] = anims;
        }

        void setFrames(EntityID id, int pos, const sf::IntRect&& anim) noexcept 
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos] = { anim };
        }

        void addFrame(EntityID id, int pos, const sf::IntRect&& anim) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos].push_back(anim);
        }

        void setCurrentAnimation(EntityID id, int pos) noexcept 
        {
            auto& animation = Component::animations[id].value();
            
            // make sure position exists
            if(animation.animations.find(pos) != animation.animations.end()) { 
                if(pos != animation.currentAnimation) {
                    animation.currentAnimation = pos;            

                    // Set the first frame to sprite
                    auto& base = Component::bases[id].value();
                    base.sprite.setTextureRect(animation.animations[pos][0]); 
                }
            }
            #ifdef ENABLE_DEBUG_MODE
            else {
                Debug::printAndThrow("ID:", id, " - position is not existed!");
            }
            #endif
        }

        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer = 500) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.nextFrameTimer = next_animation_timer;
        }

        void setStopWhenFinished(EntityID id, STOP stop = STOP::FALSE) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.stopWhenFinished = bool(stop);
        }

        void setAllowPlay(EntityID id, ALLOW allow = ALLOW::TRUE) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.allowPlay = bool(allow);
        }

        void play(EntityID id) noexcept 
        {
            auto& animation = Component::animations[id].value();
            auto& base = Component::bases[id].value();

            if(animation.allowPlay)
            {
                const int maxFrames = animation.animations[animation.currentAnimation].size();
                animation.isStarted = true;

                if(sf::Time timer = animation.clock.getElapsedTime();
                    timer >= sf::milliseconds(animation.nextFrameTimer))
                {
                    if(animation.currentFrame >= maxFrames) {
                        animation.currentFrame = 0;
                        animation.isFinished = true;
                    }

                    // get out of the function before setting a different frame
                    if(animation.isFinished && animation.stopWhenFinished) {
                        return;
                    }

                    // set the current frame
                    base.sprite.setTextureRect(animation.animations[animation.currentAnimation][animation.currentFrame]);

                    animation.currentFrame++;
                    animation.clock.restart();
                }
            }
        }

        namespace Helper
        {
            sf::IntRect extractTextureRect(const sf::IntRect&& rect) noexcept 
            {
                const int width = std::abs(rect.left - rect.width);
                const int height = std::abs(rect.top - rect.height);
                return sf::IntRect(rect.left, rect.top, width, height);
            }
        } // namespace Helper

    } // namespace Animation

    namespace Movement
    {
        bool isPrepared(EntityID id, WITH_PHYSICS with_physics)
        {
            if(not Component::bases[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - bases'");
            }

            if(not Component::movements[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - movements'");
            }

            if(not Component::animations[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - animations'");
            }

            if(bool(with_physics)) {
                if(not Component::physics[id].has_value()) {
                    throw std::runtime_error("Failed to access 'map - physics'");
                }
            }
        
            return SUCCESS;
        }

        Enum::Direction getLookingDirection(EntityID id) noexcept 
        {
            auto& movement = Component::movements[id].value();
            return movement.lookingDirection;
        }

        void moveRight(EntityID id, float speed) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(speed, 0));
            
            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to right!");
            #endif
        }

        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to right!");
            #endif
        }

        void moveLeft(EntityID id, float speed) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(-speed, 0));

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to left!");
            #endif
        }

        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(-speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to left!");
            #endif
        }


        void jump(EntityID id, unsigned int height) noexcept
        {
            auto& movement = Component::movements[id].value();
            if(not movement.isJumping && Physics::isOnGround(id)) {
                Movement::setJumping(id, JUMPING::TRUE);
                Movement::setMoving(id, MOVING::TRUE);

                auto& physics = Component::physics[id].value();
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }
        void jump(EntityID id, unsigned int height, Enum::Animation anim) noexcept
        {
            auto& movement = Component::movements[id].value();
            if(not movement.isJumping && Physics::isOnGround(id)) {
                Movement::setJumping(id, JUMPING::TRUE);
                Movement::setMoving(id, MOVING::TRUE);
                
                auto& physics = Component::physics[id].value();
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }

        void setMoving(EntityID id, MOVING moving) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isMoving = bool(moving);
        }

        void setRunning(EntityID id, RUNNING running) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isRunning = int(running);
        }

        void setJumping(EntityID id, JUMPING jumping) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isJumping = bool(jumping);
        }

        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.lookingDirection = direction;
        }

        bool getJumping(EntityID id) noexcept
        {
            auto& movement = Component::movements[id].value();
            return movement.isJumping;
        }
    } // namespace Movement

    namespace Physics
    {
        bool isPrepared(EntityID id)
        {
            if(not Component::bases[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - bases'");
            }

            if(not Component::movements[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - movements'");
            }

            if(not Component::physics[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - physics'");
            }

            return SUCCESS;
        }

        bool isMidAir(EntityID id) noexcept
        {
            auto& physics = Component::physics[id].value();
            return !physics.onGround;
        }

        bool isOnGround(EntityID id) noexcept
        {
            auto& physics = Component::physics[id].value();
            return physics.onGround;
        }

        void start(EntityID id) noexcept
        {
            auto& physics  = Component::physics[id].value();
            auto& movement = Component::movements[id].value();
            bool touchingGroundCounter = false;

            for(EntityID secondId = 0; secondId < Component::maxIndexes; secondId++) 
            {
                if(Manager::canAccess(secondId)) 
                {
                    if(secondId != id) {
                        auto collision = Physics::Helper::checkIntersections(id, secondId);
                        auto& secondType = Component::types[secondId].value(); // throwing

                        if(secondType.type == Enum::Type::BLOCK)
                        {
                            if(collision == COLLISION::TOP) {
                                touchingGroundCounter = true;
                            } 
                        }
                    }
                }
            }

            if(touchingGroundCounter) {
                physics.onGround = true;
            } else {
                physics.onGround = false;
            }

            if(not physics.onGround && not movement.isJumping) {
                Component::bases[id] ->sprite.move(0, physics.speed);
            } else if(movement.isJumping) {
                #ifdef ENABLE_DEBUG_MODE
                Debug::print("ID:", id, " - is Jumping!");
                #endif

                Component::bases[id] ->sprite.move(0, -physics.speed);

                if(sf::Time timer = physics.jumpClock.getElapsedTime();
                   timer >= sf::milliseconds(physics.maxJumpHeight))
                {
                    Movement::setJumping(id, JUMPING::FALSE);
                    physics.jumpClock.restart();
                }
            }
        }

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept
            {
                const sf::FloatRect idGlobalBounds  = Component::bases[id]        -> sprite.getGlobalBounds();
                const sf::FloatRect sidGlobalBounds = Component::bases[second_id] -> sprite.getGlobalBounds();
            
                const float idRight   = idGlobalBounds.left  + (idGlobalBounds.width / 2);
                const float idBottom  = idGlobalBounds.top   + (idGlobalBounds.height / 2);
                const float sidRight  = sidGlobalBounds.left + (sidGlobalBounds.width / 2);
                const float sidBottom = sidGlobalBounds.top  + (sidGlobalBounds.height / 2);

                if(idGlobalBounds.intersects(sidGlobalBounds)) {
                    std::vector<COLLISION> collisionArray;
                    // only 3 sides possible to touch
                    collisionArray.reserve(3); 
                    
                    // Touching on the top
                    if(sidBottom >= idGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        collisionArray.push_back(COLLISION::TOP);
                    }

                    // Touching on the right
                    if(idRight >= sidGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        collisionArray.push_back(COLLISION::RIGHT);
                    }

                    // Touching on the left
                    if(sidRight >= idGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        collisionArray.push_back(COLLISION::LEFT);
                    }

                    // Touching on the bottom
                    if(idBottom >= sidGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        collisionArray.push_back(COLLISION::BOTTOM);
                    }

                    // if hit multiple sides, return top
                    if(collisionArray.size() > 1) {
                        return COLLISION::TOP;
                    } else if(collisionArray.size() != 0) {
                        return collisionArray.at(0); 
                    }

                    #ifdef ENABLE_DEBUG_MODE
                    Debug::print("ID:", id, " overlapping with ", second_id, '!');
                    #endif
                }

                return COLLISION::NONE;
            }
        } // namespace Helper
        
    } // namespace Physics

} // namespace System