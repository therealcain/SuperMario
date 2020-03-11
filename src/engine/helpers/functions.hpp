#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// -------------------------FUNCTIONS------------------------ //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //

#include <iostream>
#include <type_traits>
#include <stdexcept>

#include "checkers.hpp"

#ifdef ENABLE_DEBUG_MODE
namespace Debug
{
    template<typename... Ts>
    constexpr std::enable_if_t<((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...)> print(Ts... args) noexcept
        // requires ((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...) // C++20
    {
        ((std::cout << args), ...);
        std::cout << std::endl;
    }

    template<typename... Ts>
    constexpr std::enable_if_t<((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...)> printAndThrow(Ts... args)
        // requires ((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...) // C++20
    {
        ((std::cout << args), ...);
        throw std::runtime_error("\n");
    }
} // namespace Debug
#endif // ENABLE_DEBUG_MODE

// Return the sum of unlimited elements
template <typename RetType, typename T, typename U>
constexpr RetType sum(T t, U u) noexcept 
{
    return static_cast<RetType>(t) + static_cast<RetType>(u);
}

// This is not being used currently
#if false
template <typename RetType, typename T, typename U, typename... Ts>
constexpr RetType sum(T t, U u, Ts... ts) noexcept
{
    return static_cast<RetType>(t) + sum<RetType>(u, ts...);
}
#endif

#endif