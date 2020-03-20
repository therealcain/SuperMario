#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <variant>
#include <any>

#include "helpers/enums.hpp"
#include "helpers/values.hpp"

// ----- Using's ------ //
using EntityID = long unsigned int; // similar to size_t

using AnimationVector = std::vector<sf::IntRect>;
using AnimationMap    = std::unordered_map<int, AnimationVector>;
using BlockPair       = std::pair<Enum::Block, Enum::Type>;
using VariantWhatType = std::variant</* Block Type */     BlockPair, 
                                     /* Mario Maturity */ Enum::Mature,
                                     /* Get out*/         bool>;
using OptVarWhatType  = std::optional<VariantWhatType>;

namespace Component
{
    // ----------- BASE ---------- //
    struct Base
    {
        sf::Texture texture;
        sf::Sprite sprite;
    };

    // ----------- TYPE ---------- //
    struct Type 
    {
        Enum::Type type;

        // assignment of what object type is
        OptVarWhatType whatType;
    }; 
    
    // ----------- ANIMATION ---------- //
    struct Animation 
    {
        sf::Clock clock;
        unsigned int nextFrameTimer = 500; // in ms

        int currentAnimation = -100;
        int currentFrame;

        bool isFinished       = false;
        bool isStarted        = false;
        bool stopWhenFinished = false;
        bool allowPlay        = true;

        AnimationMap animations;
    };

    // --------- UPDATE FUNCTION ------------ //
    using UpdateFunction  = std::function<void(EntityID)>;

    // ----------- MOVEMENT ---------- //
    struct Movement 
    {
        bool isMoving  = false;
        bool isRunning = false;
        bool isJumping = false;
        
        Enum::Direction lookingDirection;
        Enum::Direction blockedDirection;
    };

    // ----------- PHYSICS ---------- //
    struct Physics
    {
        bool onGround;
        float speed = FALL_SPEED;
        unsigned int maxJumpHeight = 400;

        sf::Clock jumpClock;
    };

    // -------- GLOBALS ------------- //
    struct GlobalVariables
    {
        std::optional<sf::Clock> clock;
        std::vector<std::any> values;
    };
} // namespace Component

// ----- Using's ----- //
using ComponentBaseMap      = std::unordered_map<EntityID, Component::Base>;
using ComponentTypeMap      = std::unordered_map<EntityID, Component::Type>;
using ComponentAnimationMap = std::unordered_map<EntityID, Component::Animation>;
using ComponentUpdateMap    = std::unordered_map<EntityID, Component::UpdateFunction>;
using ComponentMovementMap  = std::unordered_map<EntityID, Component::Movement>;
using ComponentPhysicsMap   = std::unordered_map<EntityID, Component::Physics>;
using ComponentGlobalVarMap = std::unordered_map<EntityID, Component::GlobalVariables>;

namespace Component
{
    inline EntityID maxIndexes = 0;
    inline ComponentBaseMap      bases;
    inline ComponentTypeMap      types;
    inline ComponentAnimationMap animations;
    inline ComponentUpdateMap    updates;
    inline ComponentMovementMap  movements;
    inline ComponentPhysicsMap   physics;
    inline ComponentGlobalVarMap globalVariables;
} // namespace Component

#endif