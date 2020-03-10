#include "system.hpp"
#include "helpers/values.hpp"
#include "helpers/functions.hpp"

#include <exception>
#include <cmath>

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
            /*Render*/ draw(i);
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
                /*oprator()*/ update(id);
            }
        }

        void updateAll() noexcept 
        {
            for(size_t i = 0; i < Component::updates.size(); i++) {
                /*Game*/ update(i);
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
                Debug::print("ID:", id, " - position is not exist!");
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

        sf::IntRect extractTextureRect(const sf::IntRect&& rect) noexcept 
        {
            const int width = std::abs(rect.left - rect.width);
            const int height = std::abs(rect.top - rect.height);
            return sf::IntRect(rect.left, rect.top, width, height);
        }

    } // namespace Animation

    namespace Movement
    {
        bool isPrepared(EntityID id)
        {
            if(not Component::bases[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - bases'");
            }

            if(not Component::movements[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - movements'");
            }

            if(not Component::animations[id].has_value()) {
                throw std::runtime_error("Failed to access 'map - movements'");
            }
        
            return SUCCESS;
        }

        void setMoving(EntityID id, MOVING moving) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isMoving = bool(moving);
        }

        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.lookingDirection = direction;
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
            Animation::setAllowPlay(id, ALLOW::TRUE);

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to right!");
            #endif
        }

        void moveRight(EntityID id, float speed, Enum::Animation anim, Enum::Mature maturity) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(speed, 0));

            Animation::setCurrentAnimation(id, sum<int>(anim, maturity));
            Animation::setAllowPlay(id, ALLOW::TRUE);

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
            Animation::setAllowPlay(id, ALLOW::TRUE);

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to left!");
            #endif
        }

        void moveLeft(EntityID id, float speed, Enum::Animation anim, Enum::Mature maturity) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(-speed, 0));

            Animation::setCurrentAnimation(id, sum<int>(anim, maturity));
            Animation::setAllowPlay(id, ALLOW::TRUE);

            Movement::setMoving(id, MOVING::TRUE);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);

            #ifdef ENABLE_DEBUG_MODE
            Debug::print("ID:", id, " Moving to left!");
            #endif
        }


    } // namespace Movement

} // namespace System