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


// Return the sum of unlimited elements
template <typename RetType, typename T, typename U>
constexpr RetType sum(T t, U u) noexcept 
{
    return static_cast<RetType>(t) + static_cast<RetType>(u);
}

#endif