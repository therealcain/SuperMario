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
#include <string_view>

// Return the sum of unlimited elements
template <typename RetType, typename T, typename U>
constexpr RetType sum(T t, U u) noexcept 
{
    return static_cast<RetType>(t) + static_cast<RetType>(u);
}

constexpr void myAssert(bool expr, std::string_view str) noexcept 
{
    if(expr) 
    {
        std::cout << str << std::endl;
        std::abort();
    }   
}

#endif