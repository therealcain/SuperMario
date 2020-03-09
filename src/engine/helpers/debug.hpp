#ifndef DEBUG_HPP
#define DEBUG_HPP

// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// -------------------DEBUG FUNCTIONS------------------------ //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //


#ifdef ENABLE_DEBUG_MODE

#include "checkers.hpp"

#include <iostream>
#include <type_traits>

namespace Debug
{
    template<typename... Ts>
    constexpr std::enable_if_t<((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...)> print(Ts... args) noexcept
        // requires ((std::is_arithmetic_v<Ts> || is_string_v<Ts>) && ...) // C++20
    {
        ((std::cout << args), ...);
        std::cout << std::endl;
    }
} // namespace Debug

#endif // ENABLE_DEBUG_MODE

#endif