#ifndef BLOCK_HPP
#define BLOCK_HPP

#include "../engine/components.hpp"

namespace Entity
{
    namespace Block 
    {
        void create(const sf::Vector2f&& position, Enum::Block type) noexcept;
        void hit(EntityID id) noexcept;
    } // namespace Block
} // namespace Entity

#endif