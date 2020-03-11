#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"

namespace Entity
{
    namespace Block 
    {
        void create(const sf::Vector2f&& position, Enum::Block type) noexcept;
        void hit(EntityID id) noexcept;
    } // namespace Block

    namespace Cloud {
        void create(sf::Vector2f&& position) noexcept;
    } // namespace Cloud

    namespace Coin {
        void create(sf::Vector2f&& position) noexcept;
        inline void hit(EntityID id) noexcept;
    } // namespace Coin

} // namespace Entity

#endif