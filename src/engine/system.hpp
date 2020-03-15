#ifndef SYSTEM_HPP
#define SYSTEM_HPP

#include "components.hpp"
#include "helpers/enums.hpp"
#include "helpers/checkers.hpp"

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

        namespace Helper 
        {
            inline std::vector<std::pair<EntityID, /*wait for animation to finish*/bool>> removeableIDS;
            void removeID(EntityID id, WAIT_FOR_ANIM wait_for_anim) noexcept;
        } // namespace Helper
    } // namespace Game

    // ----------- Animation ------------ //
    namespace Animation 
    {
        bool isPrepared(EntityID id);
        // this function is needed to be called in the update loop
        void play(EntityID id) noexcept;

        void addFrame(EntityID id, int pos, const sf::IntRect& anim) noexcept;
        void setCurrentAnimation(EntityID id, int pos) noexcept;
        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer) noexcept;
        void setFrames(EntityID id, int pos, const AnimationVector& anims) noexcept;
        void setFrames(EntityID id, int pos, const sf::IntRect& anim) noexcept;
        void setStopWhenFinished(EntityID id, STOP stop) noexcept;
        void setAllowPlay(EntityID id, ALLOW allow) noexcept;
        void setStarted(EntityID id, STARTED started) noexcept;
        void setFinished(EntityID id, FINISHED finished) noexcept;

        bool getAnimationFinished(EntityID id) noexcept;
        const AnimationVector* getFrames(EntityID id, int pos) noexcept;

        namespace Helper
        {
            // Helping extract the correct texture rect
            sf::IntRect extractTextureRect(const sf::IntRect& rect) noexcept;
        } // namespace Helper
    } // namespace Animation

    // ----------- Animation ------------ //
    namespace Movement 
    {
        bool isPrepared(EntityID id);
        void moveRight(EntityID id, float speed) noexcept;
        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept;
        void moveLeft(EntityID id, float speed) noexcept;
        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept;
        void jump(EntityID id, unsigned int height, FORCE force) noexcept;
        void jump(EntityID id, unsigned int height, Enum::Animation anim, FORCE force) noexcept;

        void setMoving(EntityID id, MOVING moving) noexcept;
        void setRunning(EntityID id, RUNNING running) noexcept;
        void setJumping(EntityID id, JUMPING jumping) noexcept;
        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept;

        Enum::Direction getLookingDirection(EntityID id) noexcept;
        Enum::Direction getBlockedDirection(EntityID id) noexcept;
        bool getJumping(EntityID id) noexcept;
        bool getRunning(EntityID id) noexcept;
        bool getMoving(EntityID id) noexcept;
    } // namespace Movement

    // ----------- Physics ------------ //
    namespace Physics
    {
        bool isPrepared(EntityID id);
        bool isMidAir(EntityID id) noexcept;
        bool isOnGround(EntityID id) noexcept;
        void start(EntityID id) noexcept;

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept;

            // Type Intersection
            void checkTouchedCoin(EntityID second_id, COLLISION collision);
            void checkTouchedBlock(EntityID second_id, COLLISION collision);
            void checkTouchedGoomba(EntityID id, EntityID second_id, COLLISION collision);
        } // namespace Helper
    }

} // namespace System

#endif