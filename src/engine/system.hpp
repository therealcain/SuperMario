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
            inline std::vector<std::pair<EntityID, 
                /*wait for animation to finish*/bool>> removeableIDS;
        } // namespace
    } // namespace Game

    // --------- Base ----------------- //
    namespace Base
    {
        void setState(EntityID id, Enum::State state) noexcept;

        sf::Sprite& getSprite(EntityID id) noexcept;
        Enum::State& getState(EntityID id);
    }

    // --------- Type ----------------- //
    namespace Type
    {
        void setType(EntityID id, Enum::Type type) noexcept;
        void setWhatType(EntityID id, BlockPair&& block_pair) noexcept;
        void setWhatType(EntityID id, Enum::Mature maturity) noexcept;

        Enum::Type getType(EntityID id) noexcept;
        const BlockPair& getBlockPair(EntityID id) noexcept;
        Enum::Mature getMaturity(EntityID id) noexcept;
    }

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
        const AnimationVector& getFrames(EntityID id, int pos);
        int getCurrentAnimation(EntityID id) noexcept;
        bool getStarted(EntityID id) noexcept;
        bool getFinished(EntityID id) noexcept;

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
        
        void setOnGround(EntityID id, bool on_ground) noexcept;
        void setSpeed(EntityID id, float speed) noexcept;
        void setRigidbody(EntityID id, bool is_rigidbody) noexcept;

        bool isMidAir(EntityID id) noexcept;
        bool getOnGround(EntityID id) noexcept;
        float getSpeed(EntityID id) noexcept;
        unsigned int getMaxJumpHeight(EntityID id) noexcept;
        bool getRigidbody(EntityID id) noexcept;

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept;

            void checkFalling(EntityID id) noexcept;
            void checkTouchingObject(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedCoin(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedBlock(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedGoomba(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedSpiny(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedMushroom(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool checkTouchedFlower(EntityID id, EntityID second_id, COLLISION collision) noexcept;
            bool fireTouchedEnemy(EntityID id, EntityID second_id, COLLISION collision) noexcept;
        } // namespace Helper
    } // namespace Physics

    // ----------- GlobalVariables ------------ //
    namespace GlobalVariables
    {
        template<typename T>
        constexpr std::enable_if_t<std::is_arithmetic_v<T>> addAny(EntityID id, T value) noexcept 
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
        constexpr std::enable_if_t<std::is_arithmetic_v<T>> setAny(EntityID id, T value, size_t vector_position) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values[vector_position] = value;
        }

        template<typename T>
        constexpr std::enable_if_t<std::is_arithmetic_v<T>> setLastAny(EntityID id, T value) noexcept 
        {
            auto& global = Component::globalVariables[id];
            global.values[global.values.size() - 1] = value;
        }

        template<typename T>
        constexpr std::enable_if_t<std::is_arithmetic_v<T>> setAnyOnce(EntityID id, T value, size_t vector_position) noexcept 
        {
            auto& global = Component::globalVariables[id];
            if(not global.values[vector_position].has_value()) {
                global.values[vector_position] = value;
            }
        }

        sf::Clock& getClock(EntityID id) noexcept;
        
        template<typename T, typename = std::enable_if<std::is_arithmetic_v<T>>>
        constexpr T getLastAny(EntityID id) noexcept 
        {
            auto& global = Component::globalVariables[id];
            return std::any_cast<T>(global.values[global.values.size() - 1]);
        }

        template<typename T, typename = std::enable_if<std::is_arithmetic_v<T>>>
        constexpr T getAny(EntityID id, size_t vector_position) noexcept
        {
            auto& global = Component::globalVariables[id];
            return std::any_cast<T>(global.values[vector_position]);
        }

        bool existsAny(EntityID id, size_t vector_position) noexcept;
    } // namespace GlobalVariables

} // namespace System

#endif