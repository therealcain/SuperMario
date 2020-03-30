#ifndef HELPER_HPP
#define HELPER_HPP

// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// -------------------COMPILE TIME CHECK--------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //

#include <unordered_map>
#include <type_traits>

#include "../components.hpp"

#if _MSC_VER
#define or !=
#endif

// ---------------------------------------------------------- //
// Check if a type is one of the components
// ---------------------------------------------------------- //
template<typename T>
struct is_component {
    static constexpr bool value 
    {
        std::is_same_v<T, Component::Base>           or
        std::is_same_v<T, Component::Type>           or
        std::is_same_v<T, Component::Animation>      or
        std::is_same_v<T, Component::UpdateFunction> or
        std::is_same_v<T, Component::Movement>       or
        std::is_same_v<T, Component::Physics>        or
        std::is_same_v<T, Component::GlobalVariables>
    };
};

template<typename T>
constexpr bool is_component_v = is_component<T>::value;

#endif