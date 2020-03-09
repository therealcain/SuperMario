#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <SFML/Graphics.hpp>

#include "components.hpp"
#include "helpers/checkers.hpp"
#include "helpers/enums.hpp"

namespace Manager 
{
    EntityID create(const std::string&& png);
    bool canAccess(EntityID id) noexcept;
    void remove(EntityID id) noexcept;

    template<typename T>
    constexpr std::enable_if_t<is_component_v<T>> addComponent(EntityID id, BE_NULL be_null) noexcept
    {
        if constexpr(std::is_same_v<T, Component::Base>) {
            if(bool(be_null)) {
                Component::bases[id].reset();
            } else {
                Component::bases[id] = T();
            }
        }  
        else if constexpr(std::is_same_v<T, Component::Type>) {
            if(bool(be_null)) {
                Component::types[id].reset();
            } else {
                Component::types[id] = T();
            }
        }
        else if constexpr(std::is_same_v<T, Component::Animation>) {
            if(bool(be_null)) {
                Component::animations[id].reset();
            } else {
                Component::animations[id] = T();
            }
        }
        else if constexpr(std::is_same_v<T, Component::UpdateFunction>) {
            Component::updates[id].reset();
        }
    }
}   

#endif