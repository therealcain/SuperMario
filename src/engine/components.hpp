#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <SFML/Graphics.hpp>

#include <unordered_map>
#include <vector>
#include <optional>
#include <functional>
#include <variant>

#include "helpers/enums.hpp"

// ----- Using's ------ //
using EntityID = long unsigned int; // similar to size_t

using AnimationVector = std::vector<sf::IntRect>;
using AnimationMap    = std::unordered_map<int, AnimationVector>;
using VariantWhatType = std::variant</* Block Type */     Enum::Block, 
                                     /* Mario Maturity */ Enum::Mature>;
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

        bool isFinished = false;
        bool isStarted  = false;
        bool stopWhenFinished = false;
        bool allowPlay = true;

        AnimationMap animations;
    };

    // --------- UPDATE FUNCTION ------------ //
    using UpdateFunction  = std::function<void(EntityID)>;

    // ----------- MOVEMENT ---------- //
    struct Movement 
    {
        bool isMoving  = false;
        bool isRunning = false;
        
        Enum::Direction lookingDirection;
    };

} // namespace Component

// ----- Using's ----- //
using ComponentBaseOpt      = std::optional<Component::Base>;
using ComponentTypeOpt      = std::optional<Component::Type>;
using ComponentAnimationOpt = std::optional<Component::Animation>;
using ComponentUpdateOpt    = std::optional<Component::UpdateFunction>;
using ComponentMovementOpt  = std::optional<Component::Movement>;

using ComponentBaseMap      = std::unordered_map<EntityID, ComponentBaseOpt>;
using ComponentTypeMap      = std::unordered_map<EntityID, ComponentTypeOpt>;
using ComponentAnimationMap = std::unordered_map<EntityID, ComponentAnimationOpt>;
using ComponentUpdateMap    = std::unordered_map<EntityID, ComponentUpdateOpt>;
using ComponentMovementMap  = std::unordered_map<EntityID, ComponentMovementOpt>;

namespace Component
{
    inline EntityID maxIndexes = 0;
    inline ComponentBaseMap      bases;
    inline ComponentTypeMap      types;
    inline ComponentAnimationMap animations;
    inline ComponentUpdateMap    updates;
    inline ComponentMovementMap  movements;
} // namespace Component

#endif