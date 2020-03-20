#ifndef MANAGER_HPP
#define MANAGER_HPP

#include <SFML/Graphics.hpp>

#include "components.hpp"
#include "helpers/checkers.hpp"
#include "helpers/enums.hpp"

namespace Manager 
{
    EntityID create(const std::string& png);
    bool canAccess(EntityID id) noexcept;
    void remove(EntityID id) noexcept;

    template<typename T>
    constexpr std::enable_if_t<is_component_v<T>> addComponent(EntityID id) noexcept
    {
        if constexpr(std::is_same_v<T, Component::Base>) {
            Component::bases[id] = T();
        }  
        else if constexpr(std::is_same_v<T, Component::Type>) {
            Component::types[id] = T();
        }
        else if constexpr(std::is_same_v<T, Component::Animation>) {
            Component::animations[id] = T();
        }
        else if constexpr(std::is_same_v<T, Component::Movement>) {
            Component::movements[id] = T();
        }
        else if constexpr(std::is_same_v<T, Component::Physics>) {
            Component::physics[id] = T();
        }
        else if constexpr(std::is_same_v<T, Component::GlobalVariables>){
            Component::globalVariables[id] = T();
        }
    }
}   

#endif