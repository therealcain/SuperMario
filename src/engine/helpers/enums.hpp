#ifndef ENUMS_HPP
#define ENUMS_HPP

// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ------------------------Enums----------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //
// ---------------------------------------------------------- //

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

    // --------- ANIMATIONS -------- //
    enum class Animation : int {
        NONE    = -2,
        DEAD    = -1,
        DEFAULT =  0,
        IDLE_RIGHT  , IDLE_LEFT  , IDLE  ,
        WALK_RIGHT  , WALK_LEFT  , WALK  ,
        RUN_RIGHT   , RUN_LEFT   , RUN   ,
        JUMP_RIGHT  , JUMP_LEFT  , JUMP  ,
        CROUCH_RIGHT, CROUCH_LEFT, CROUGH,
        SHOOT_RIGHT , SHOOT_LEFT , SHOOT,
        END
    };

    // ----------- BLOCK ----------- //
    enum class Block {
        EMPTY,
        BRICK,
        QUESTION_MARK
    };

    // ----------- MATURE ----------- //
    // this enum class is going to be multiplied for the animation
    // let's say there is a WALK_RIGHT animation, but how do we know how
    // mature the player actually is ?
    // so it's Mature + Animation
    // CHILD   + WALK_RIGHT = 0  + 4 = 4    THIS IS ANIMATION 4 for Child
    // TEENAGE + WALK_RIGHT = 20 + 4 = 24   THIS IS ANIMATION 4 for teenage
    // ADULT   + WALK_RIGHT = 40 + 4 = 44   THIS IS ANIMATION 4 for Adult
    enum class Mature : int {
        CHILD   = 0,
        TEENAGE = int(Animation::END),    // Big red mario
        ADULT   = int(Animation::END) * 2 // Big white mario that can shoot fire balls
                                          // White mario is the same as Teenage but with Color Manipulation
    };

    // ----------- DIRECTION ---------- //
    enum class Direction {
        RIGHT,
        LEFT,
        TOP,
        BOTTOM,
        NONE
    };

    // ----------- BOOLEAN ---------- //
    enum class BOOLEAN : bool {
        TRUE  = true,
        FALSE = false
    };
    
} // namespace Enum


// ----- Using's ----- //
using FORCE         = Enum::BOOLEAN;
using WAIT_FOR_ANIM = Enum::BOOLEAN;
using IS_PLAYER     = Enum::BOOLEAN;
using GET_FROM      = Enum::BOOLEAN;
using COLLISION     = Enum::Direction;
    
#endif