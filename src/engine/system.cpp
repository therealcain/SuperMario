#include "system.hpp"
#include "helpers/values.hpp"

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
            animation.currentAnimation = pos;

            // Set the first frame to sprite
            auto& base = Component::bases[id].value();
            base.sprite.setTextureRect(animation.animations[pos][0]); 
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
    
} // namespace System