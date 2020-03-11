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

// ---------------------------------------------------------- //
// Make sure a type is indeed a std:unordered_map
// ---------------------------------------------------------- //
// no usage for it currently
#if false
template<typename KEY>
struct is_unordered_map 
    : std::false_type {};

template<typename KEY, typename VALUE>
struct is_unordered_map<std::unordered_map<KEY, VALUE>> 
    : std::true_type {};
#endif

// ---------------------------------------------------------- //
// Decay and remove the pointer from a type
// ---------------------------------------------------------- //
template<typename T>
struct decay_pointer 
{
    using type = std::decay_t<std::remove_pointer_t<T>>;
};

template<typename T>
using decay_pointer_t = typename decay_pointer<T>::type;

// ---------------------------------------------------------- //
// Check if type is a string
// ---------------------------------------------------------- //
template<typename T>
struct is_c_str // C char (char*)
{
    static constexpr bool value = std::is_same_v<decay_pointer_t<T>, char>;
};

template<typename T>
constexpr bool is_c_str_v = is_c_str<T>::value;

template<typename T>
struct is_cpp_str // std::string
{
    static constexpr bool value = std::is_same_v<std::decay_t<T>, std::string>;
};

template<typename T>
constexpr bool is_cpp_str_v = is_cpp_str<T>::value;

template<typename T>
struct is_string // C string and C++ std::string
{
    static constexpr bool value 
    {
        is_c_str_v<T>  or
        is_cpp_str_v<T>
    };
};

template<typename T>
constexpr bool is_string_v = is_string<T>::value;

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
        std::is_same_v<T, Component::Physics>
    };
};

template<typename T>
constexpr bool is_component_v = is_component<T>::value;


#endif