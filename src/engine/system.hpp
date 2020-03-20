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

        void removeID(EntityID id, WAIT_FOR_ANIM wait_for_anim) noexcept;

        namespace {
            inline std::vector<std::pair<EntityID, /*wait for animation to finish*/bool>> removeableIDS;
        } // namespace
    } // namespace Game

    // ----------- Animation ------------ //
    namespace Animation 
    {
        // this function is needed to be called in the update loop
        void play(EntityID id) noexcept;

        void addFrame(EntityID id, int pos, const sf::IntRect& anim) noexcept;
        void setCurrentAnimation(EntityID id, int pos) noexcept;
        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer) noexcept;
        void setFrames(EntityID id, int pos, const AnimationVector& anims) noexcept;
        void setFrames(EntityID id, int pos, const sf::IntRect& anim) noexcept;
        void setStopWhenFinished(EntityID id, bool stop) noexcept;
        void setAllowPlay(EntityID id, bool allow) noexcept;
        void setStarted(EntityID id, bool started) noexcept;
        void setFinished(EntityID id, bool finished) noexcept;

        bool getAnimationFinished(EntityID id) noexcept;
        const AnimationVector* getFrames(EntityID id, int pos) noexcept;
        int getCurrentAnimation(EntityID id) noexcept;

        namespace Helper
        {
            // Helping extract the correct texture rect
            sf::IntRect extractTextureRect(const sf::IntRect& rect) noexcept;
        } // namespace Helper
    } // namespace Animation

    // ----------- Movement ------------ //
    namespace Movement 
    {
        void moveRight(EntityID id, float speed) noexcept;
        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept;
        void moveLeft(EntityID id, float speed) noexcept;
        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept;
        void jump(EntityID id, unsigned int height, FORCE force);
        void jump(EntityID id, unsigned int height, Enum::Animation anim, FORCE force);

        void setMoving(EntityID id, bool moving) noexcept;
        void setRunning(EntityID id, bool running) noexcept;
        void setJumping(EntityID id, bool jumping) noexcept;
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
        void start(EntityID id) noexcept;

        bool isMidAir(EntityID id) noexcept;
        bool getOnGround(EntityID id) noexcept;
        float getSpeed(EntityID id) noexcept;
        unsigned int getMaxJumpHeight(EntityID id) noexcept;

        void setOnGround(EntityID id, bool on_ground) noexcept;

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept;
        } // namespace Helper
    } // namespace Physics

    // ----------- GlobalVariables ------------ //
    namespace GlobalVariables
    {
        template<typename T>
        inline void addAny(EntityID id, T value) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values.push_back(value);
        }

        inline void addAny(EntityID id) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values.push_back({});
        }

        template<typename T>
        inline void setAny(EntityID id, T value, size_t vector_position) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values[vector_position] = value;
        }

        template<typename T>
        inline void setLastAny(EntityID id, T value) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values[global.values.size() - 1] = value;
        }

        template<typename T>
        inline void setAnyOnce(EntityID id, T value, size_t vector_position) noexcept 
        {
            auto& global = Component::globalVariables[id];
            if(not global.values[vector_position].has_value()) {
                global.values[vector_position] = value;
            }
        }

        sf::Clock& getClock(EntityID id) noexcept;
        
        template<typename T>
        inline T getLastAny(EntityID id) noexcept 
        {
            auto& global = Component::globalVariables[id];
            return std::any_cast<T>(global.values[global.values.size() - 1]);
        }

        template<typename T>
        inline T getAny(EntityID id, size_t vector_position) noexcept
        {
            auto& global = Component::globalVariables[id];
            return std::any_cast<T>(global.values[vector_position]);
        }
    } // namespace GlobalVariables

} // namespace System

#endif