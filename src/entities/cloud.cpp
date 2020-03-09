#include "cloud.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"

namespace Entity {
    namespace Cloud {
        void create(sf::Vector2f&& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/cloud.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::MARIO;

            
        }
    } // namespace Cloud
} // namespace Entity