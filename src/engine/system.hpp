#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "components.hpp"
#include "helpers/enums.hpp"

namespace System 
{
    // ----------- Render ------------ //
    class Render
    {
    public:
        constexpr Render(sf::RenderWindow& window) 
            : m_window(window) {}
        void draw(EntityID id) noexcept;
        void drawAll() noexcept;

    private:
        sf::RenderWindow& m_window;
    };

    // --------- Game Loop ------------- //
    namespace Game
    {
        void update(EntityID id) noexcept;
        void updateAll() noexcept;
    }

    // ----------- Animation ------------ //
    namespace Animation 
    {
        bool isPrepared(EntityID id);
        void setFrames(EntityID id, int pos, const AnimationVector&& anims) noexcept;
        void setFrames(EntityID id, int pos, const sf::IntRect&& anim) noexcept;
        void addFrame(EntityID id, int pos, const sf::IntRect&& anim) noexcept;
        void setCurrentAnimation(EntityID id, int pos) noexcept;
        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer) noexcept;
        void setStopWhenFinished(EntityID id, STOP stop) noexcept;
        void setAllowPlay(EntityID id, ALLOW allow) noexcept;
        // this function is needed to be called in the update loop
        void play(EntityID id) noexcept;

        // Helping extract the correct texture rect
        sf::IntRect extractTextureRect(const sf::IntRect&& rect) noexcept;
    } // namespace Animation

    // ----------- Animation ------------ //
    namespace Movement 
    {
        bool isPrepared(EntityID id, WITH_PHYSICS with_physics);
        void setMoving(EntityID id, MOVING moving) noexcept;
        void setRunning(EntityID id, RUNNING running) noexcept;
        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept;
        Enum::Direction getLookingDirection(EntityID id) noexcept;
        void moveRight(EntityID id, float speed) noexcept;
        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept;
        void moveRight(EntityID id, float speed, Enum::Animation anim, Enum::Mature maturity) noexcept;
        void moveLeft(EntityID id, float speed) noexcept;
        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept;
        void moveLeft(EntityID id, float speed, Enum::Animation anim, Enum::Mature maturity) noexcept;
        void jump(EntityID id, float speed, unsigned int height) noexcept;
        void jump(EntityID id, float speed, unsigned int height, Enum::Animation anim) noexcept;
        void jump(EntityID id, float speed, unsigned int height, Enum::Animation anim, Enum::Mature maturity) noexcept;
    }

    // ----------- Physics ------------ //
    namespace Physics
    {
        bool isPrepared(EntityID id);
        void setSpeed(EntityID id, float speed) noexcept;
        bool isMidAir(EntityID id) noexcept;
        bool isOnGround(EntityID id) noexcept;
        void start(EntityID id) noexcept;

        COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept;
    }

} // namespace System

#endif