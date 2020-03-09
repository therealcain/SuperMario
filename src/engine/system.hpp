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
        // this function is allowed to be called in the game loop
        void setCurrentAnimation(EntityID id, int pos) noexcept;
        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer) noexcept;
        void setStopWhenFinished(EntityID id, STOP stop) noexcept;
        void setAllowPlay(EntityID id, ALLOW allow) noexcept;
        // this function is needed to be called in the game loop
        void play(EntityID id) noexcept;

        // Helping extract the correct texture rect
        sf::IntRect extractTextureRect(const sf::IntRect&& rect) noexcept;
    } // namespace Animation

} // namespace System

#endif