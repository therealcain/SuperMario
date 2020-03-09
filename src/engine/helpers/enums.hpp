#ifndef ENUMS_HPP
#define ENUMS_HPP

// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ------------------------Enums----------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //

#include <optional>

namespace Enum 
{
    // ----------- TYPE ----------- //
    enum class Type {
        NONE = -1,
        MARIO = 0,
        ONEUP,
        BLOCK,
        CLOUD,
        COIN,
        FIRE,
        FLOWER,
        GOOMBA,
        MUSHROOM,
        SPINY,
        STAR
    };

    // ----------- BLOCK ----------- //
    enum class Block {
        EMPTY,
        BRICK,
        QUESTION_MARK
    };

    // ----------- BOOLEAN ---------- //
    enum class BOOLEAN : bool {
        TRUE  = true,
        FALSE = false
    };
    
} // namespace Enum

// ----- Using's ----- //
using BlockOpt = std::optional<Enum::Block>;

using BE_NULL  = Enum::BOOLEAN;
using STOP     = Enum::BOOLEAN;
using ALLOW    = Enum::BOOLEAN;

#endif