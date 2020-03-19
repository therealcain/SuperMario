#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <variant>

#include "helpers/enums.hpp"
#include "helpers/values.hpp"

// ----- Using's ------ //
using EntityID = long unsigned int; // similar to size_t

using AnimationVector = std::vector<sf::IntRect>;
using AnimationMap    = std::unordered_map<int, AnimationVector>;
using PairIsPlayerID  = std::pair<IS_PLAYER, std::optional<EntityID>>;
using VariantWhatType = std::variant</* Block Type */     Enum::Block, 
                                     /* Mario Maturity */ Enum::Mature,
                                     /* Mario/Enemy+ID */ PairIsPlayerID>;
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

    // ----------- Global ------------- //
    struct Global
    {
        sf::Clock clock;
        std::optional<bool> moveLeft;
        std::optional<bool> lookingDirection;
    };
} // namespace Component

// ----- Using's ----- //
using ComponentBaseOpt      = std::optional<Component::Base>;
using ComponentTypeOpt      = std::optional<Component::Type>;
using ComponentAnimationOpt = std::optional<Component::Animation>;
using ComponentUpdateOpt    = std::optional<Component::UpdateFunction>;
using ComponentMovementOpt  = std::optional<Component::Movement>;
using ComponentPhysicsOpt   = std::optional<Component::Physics>;
using ComponentGlobalOpt       = std::optional<Component::Global>;

using ComponentBaseMap      = std::unordered_map<EntityID, ComponentBaseOpt>;
using ComponentTypeMap      = std::unordered_map<EntityID, ComponentTypeOpt>;
using ComponentAnimationMap = std::unordered_map<EntityID, ComponentAnimationOpt>;
using ComponentUpdateMap    = std::unordered_map<EntityID, ComponentUpdateOpt>;
using ComponentMovementMap  = std::unordered_map<EntityID, ComponentMovementOpt>;
using ComponentPhysicsMap   = std::unordered_map<EntityID, ComponentPhysicsOpt>;
using ComponentGlobalMap    = std::unordered_map<EntityID, ComponentGlobalOpt>;

namespace Component
{
    inline EntityID maxIndexes = 0;
    inline ComponentBaseMap      bases;
    inline ComponentTypeMap      types;
    inline ComponentAnimationMap animations;
    inline ComponentUpdateMap    updates;
    inline ComponentMovementMap  movements;
    inline ComponentPhysicsMap   physics;
    inline ComponentGlobalMap    globals;
} // namespace Component

#endif