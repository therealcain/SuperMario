#ifndef COIN_HPP
#define COIN_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"

namespace Entity {
    namespace Coin {
        void create(sf::Vector2f&& position) noexcept;
        inline void hit(EntityID id) noexcept;
    } // namespace Coin
} // namespace Entity

#endif