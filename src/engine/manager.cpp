#include "manager.hpp"
#include "helpers/functions.hpp"
#include "helpers/values.hpp"

#include <exception>
#include <iostream>

namespace Manager
{
    EntityID create(const std::string& png)
    {
        EntityID currentID = Component::bases.size();

        addComponent<Component::Base>(currentID);
        addComponent<Component::Type>(currentID);

        if(not Component::bases[currentID].texture.loadFromFile(png)) {
            throw std::runtime_error(std::string("Failed to load " + png));
        }

        Component::bases[currentID].sprite.setTexture(Component::bases[currentID].texture);

        #ifdef ENABLE_DEBUG_MODE
        std::cout << "ID: " << currentID << " Created! - " << png << std::endl;
        #endif
        
        return currentID;
    }

    bool canAccess(EntityID id) noexcept
    {
        // Bases is a must for all of the components
        // if it doesn't have any value, the other 
        // components will not respond well.
        if(Component::bases.find(id) != Component::bases.end()) {
            return true;
        }

        return false;
    }

    void remove(EntityID id) noexcept 
    {
        if(Manager::canAccess(id))
        {
            Component::bases.erase(id);
            Component::types.erase(id);
            Component::animations.erase(id);
            Component::movements.erase(id);
            Component::physics.erase(id);
            Component::updates.erase(id);

            #ifdef ENABLE_DEBUG_MODE
            std::cout << "ID: " << id << " Removed! - " << std::endl;
            #endif
        } 
    }
} // namespace Manager