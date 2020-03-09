#ifndef CLOUD_HPP
#define CLOUD_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"

namespace Entity {
    namespace Cloud {
        void create(sf::Vector2f&& position) noexcept;
    } // namespace Cloud
} // namespace Entity

#endif