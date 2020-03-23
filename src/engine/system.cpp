#include "system.hpp"
#include "helpers/values.hpp"
#include "helpers/functions.hpp"
#include "manager.hpp"
#include "../entities/entities.hpp"

#include <exception>
#include <cmath>
#include <vector>

namespace System
{
    // ----------- Render ------------ //
    void Render::draw(EntityID id) noexcept 
    {
        if(Manager::canAccess(id)) {
            m_window.draw(System::Base::getSprite(id));
        }
    }

    void Render::drawAll() noexcept 
    {
        for(auto[id, base] : Component::bases)
        {
            if(Manager::canAccess(id)) {
                m_window.draw(base.sprite);   
            }
        }
    }


    // ----------- Game Loop ------------ //
    namespace Game
    {
        void update(EntityID id) noexcept 
        {
            if(Manager::canAccess(id)) 
            {
                auto& update = Component::updates[id];
                update(id);
            }
        }

        void updateAll() noexcept 
        {
            for(auto[id, update] : Component::updates)
            {
                if(Manager::canAccess(id)) {
                    update(id);
                }
            }

            /* first  = EntityID
               second = waiting for animation*/
            for(auto& i : removeableIDS) 
            {
                if(Manager::canAccess(i.first)) 
                {
                    if(i.second == true) 
                    {
                        if(Animation::getAnimationFinished(i.first) == true) {
                            Manager::remove(i.first);
                        }
                    } 
                    else {
                        Manager::remove(i.first);
                    }
                }
            }
        }

        void removeID(EntityID id, WAIT_FOR_ANIM wait_for_anim) noexcept {
            removeableIDS.push_back( std::make_pair(id, bool(wait_for_anim)) );
        }
    } // namespace Game

    // --------- Base ----------------- //
    namespace Base
    {
        void setState(EntityID id, Enum::State state) noexcept
        {
            auto& base = Component::bases[id];
            base.state = state;
        }

        sf::Sprite& getSprite(EntityID id) noexcept
        {
            auto& base = Component::bases[id];
            return base.sprite;
        }

        Enum::State& getState(EntityID id)
        {
            auto& base = Component::bases[id];
            if(not base.state.has_value()) {
                throw std::runtime_error("state doesn't have a value!");
            }

            return base.state.value();
        }
    }

    // --------- Type ----------------- //
    namespace Type
    {
        void setType(EntityID id, Enum::Type type) noexcept
        {
            auto& r_type = Component::types[id];
            r_type.type = type;
        }

        void setWhatType(EntityID id, BlockPair&& block_pair) noexcept
        {
            auto& type = Component::types[id];
            type.whatType = block_pair;
        }

        void setWhatType(EntityID id, Enum::Mature maturity) noexcept
        {
            auto& type = Component::types[id];
            type.whatType = maturity;
        }

        Enum::Type getType(EntityID id) noexcept
        {
            const auto& type = Component::types[id];
            return type.type;
        }

        const BlockPair& getBlockPair(EntityID id) noexcept
        {
            const auto& type = Component::types[id];
            return std::get<BlockPair>(*type.whatType);
        }

        Enum::Mature getMaturity(EntityID id) noexcept
        {
            const auto& type = Component::types[id];
            return std::get<Enum::Mature>(*type.whatType);
        }
    }

    // ----------- Animation ------------ //
    namespace Animation 
    {
        void setFrames(EntityID id, int pos, const AnimationVector& anims) noexcept 
        {
            auto& animation = Component::animations[id];
            animation.animations[pos] = anims;
        }

        void setFrames(EntityID id, int pos, const sf::IntRect& anim) noexcept 
        {
            auto& animation = Component::animations[id];
            animation.animations[pos] = { anim };
        }

        void addFrame(EntityID id, int pos, const sf::IntRect& anim) noexcept
        {
            auto& animation = Component::animations[id];
            animation.animations[pos].push_back(anim);
        }

        void setCurrentAnimation(EntityID id, int pos) noexcept 
        {
            auto& animation = Component::animations[id];
            
            // make sure position exists
            if(animation.animations.find(pos) != animation.animations.end()) 
            { 
                if(pos != animation.currentAnimation) 
                {
                    Animation::setStarted(id, false);
                    Animation::setFinished(id, false);

                    animation.currentAnimation = pos;

                    // Set the first frame to sprite
                    System::Base::getSprite(id).setTextureRect(animation.animations[pos][0]);
                }
            }
            #ifdef ENABLE_DEBUG_MODE
            else {
                std::cerr << "ID:" << id << " Position is not exist!" << std::endl;
            }
            #endif
        }

        void setNextAnimationTimer(EntityID id, unsigned int next_animation_timer = 500) noexcept
        {
            auto& animation = Component::animations[id];
            animation.nextFrameTimer = next_animation_timer;
        }

        void setStopWhenFinished(EntityID id, bool stop) noexcept
        {
            auto& animation = Component::animations[id];
            animation.stopWhenFinished = stop;
        }

        void setAllowPlay(EntityID id, bool allow) noexcept
        {
            auto& animation = Component::animations[id];
            animation.allowPlay = allow;
        }

        void setStarted(EntityID id, bool started) noexcept
        {
            auto& animation = Component::animations[id];
            animation.isStarted = started;
        }

        void setFinished(EntityID id, bool finished) noexcept
        {
            auto& animation = Component::animations[id];
            animation.isFinished = finished;
        }

        void play(EntityID id) noexcept 
        {
            auto& animation = Component::animations[id];

            if(animation.allowPlay)
            {
                const int maxFrames = animation.animations[animation.currentAnimation].size();
                Animation::setStarted(id, true);

                if(sf::Time timer = animation.clock.getElapsedTime();
                    timer >= sf::milliseconds(animation.nextFrameTimer))
                {
                    if(animation.currentFrame >= maxFrames) 
                    {
                        animation.currentFrame = 0;
                        Animation::setFinished(id, true);
                    }

                    // get out of the function before setting a different frame
                    if(animation.isFinished && animation.stopWhenFinished) {
                        return;
                    }

                    // set the current frame
                    System::Base::getSprite(id).setTextureRect(animation.animations[animation.currentAnimation][animation.currentFrame]);

                    animation.currentFrame++;
                    animation.clock.restart();
                }
            }
        }

        bool getAnimationFinished(EntityID id) noexcept
        {
            const auto& animation = Component::animations[id];
            return animation.isFinished;
        }

        const AnimationVector& getFrames(EntityID id, int pos)
        {
            const auto& animation = Component::animations[id];
            if(animation.animations.find(pos) == animation.animations.end()) { 
                throw std::runtime_error("Animation Vector doesn't exists!");
            }

            return animation.animations.at(pos);
        }   

        int getCurrentAnimation(EntityID id) noexcept
        {
            const auto& animation = Component::animations[id];
            return animation.currentAnimation;
        }

        bool getStarted(EntityID id) noexcept
        {
            const auto& animation = Component::animations[id];
            return animation.isStarted;
        }

        bool getFinished(EntityID id) noexcept
        {
            const auto& animation = Component::animations[id];
            return animation.isFinished;
        }

        namespace Helper
        {
            sf::IntRect extractTextureRect(const sf::IntRect& rect) noexcept 
            {
                const int width = std::abs(rect.left - rect.width);
                const int height = std::abs(rect.top - rect.height);
                return sf::IntRect(rect.left, rect.top, width, height);
            }
        } // namespace Helper

    } // namespace Animation

    // ----------- Movement ------------ //
    namespace Movement
    {
        void moveRight(EntityID id, float speed) noexcept
        {
            System::Base::getSprite(id).move(sf::Vector2f(speed, 0));
            
            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            System::Base::getSprite(id).move(sf::Vector2f(speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveLeft(EntityID id, float speed) noexcept
        {
            System::Base::getSprite(id).move(sf::Vector2f(-speed, 0));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);
        }

        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            System::Base::getSprite(id).move(sf::Vector2f(-speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);
        }


        void jump(EntityID id, unsigned int height, FORCE force)
        {
            #ifdef ENABLE_DEBUG_MODE
            if(Component::physics.find(id) == Component::physics.end()) {
                throw std::runtime_error("Please add physics component to enable jumping!");
            }
            #endif

            auto& movement = Component::movements[id];
            if((not movement.isJumping && Physics::getOnGround(id)) || bool(force)) 
            {
                Movement::setJumping(id, true);

                auto& physics = Component::physics[id];
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }

        void jump(EntityID id, unsigned int height, Enum::Animation anim, FORCE force)
        {
            #ifdef ENABLE_DEBUG_MODE
            if(Component::physics.find(id) == Component::physics.end()) {
                throw std::runtime_error("Please add physics component to enable jumping!");
            }
            #endif

            auto& movement = Component::movements[id];
            if((not movement.isJumping && Physics::getOnGround(id)) || bool(force)) 
            {
                Movement::setJumping(id, true);
                
                auto& physics = Component::physics[id];
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }

        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept
        {
            auto& movement = Component::movements[id];
            movement.lookingDirection = direction;
        }

        void setBlockedDirection(EntityID id, Enum::Direction direction) noexcept 
        {
            auto& movement = Component::movements[id];
            movement.blockedDirection = direction;
        }

        void setMoving(EntityID id, bool moving) noexcept
        {
            auto& movement = Component::movements[id];
            movement.isMoving = moving;
        }

        void setRunning(EntityID id, bool running) noexcept
        {
            auto& movement = Component::movements[id];
            movement.isRunning = running;
        }

        void setJumping(EntityID id, bool jumping) noexcept
        {
            auto& movement = Component::movements[id];
            movement.isJumping = jumping;
        }

        Enum::Direction getLookingDirection(EntityID id) noexcept 
        {
            const auto& movement = Component::movements[id];
            return movement.lookingDirection;
        }

        Enum::Direction getBlockedDirection(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id];
            return movement.blockedDirection;
        }

        bool getJumping(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id];
            return movement.isJumping;
        }

        bool getRunning(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id];
            return movement.isRunning;
        }

        bool getMoving(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id];
            return movement.isMoving;
        }
    } // namespace Movement

    // ----------- Physics ------------ //
    namespace Physics
    {
        void start(EntityID id) noexcept
        {
            bool touchingGround = false;
            Enum::Direction blockedDirection = Enum::Direction::NONE;

            for(EntityID secondID = 0; secondID < Component::maxIndexes; secondID++) 
            {
                if(Manager::canAccess(secondID)) 
                {
                    if(secondID != id) 
                    {
                        COLLISION collision = Physics::Helper::checkIntersections(id, secondID);
                        const bool rigid = Physics::getRigidbody(secondID);

                        if(rigid)
                        {
                            if(collision == COLLISION::TOP) 
                            {
                                touchingGround = true;
                                blockedDirection = Enum::Direction::TOP;
                            } 
                            else if(collision == COLLISION::BOTTOM) 
                            {
                                Movement::setJumping(id, false);
                                blockedDirection = Enum::Direction::BOTTOM;
                            }
                            else if(collision == COLLISION::RIGHT) {
                                blockedDirection = Enum::Direction::RIGHT;
                            }
                            else if(collision == COLLISION::LEFT) {
                                blockedDirection = Enum::Direction::LEFT;
                            } 
                        }

                        Physics::Helper::checkTouchingObject(id, secondID, collision);
                    }
                }
            }

            if(touchingGround) {
                Physics::setOnGround(id, true);
            } 
            else {
                Physics::setOnGround(id, false);
            }

            Movement::setBlockedDirection(id, blockedDirection);
            Physics::Helper::checkFalling(id);
        }

        void setSpeed(EntityID id, float speed) noexcept
        {
            auto& physics = Component::physics[id];
            physics.speed = speed;
        }
        
        void setOnGround(EntityID id, bool on_ground) noexcept
        {
            auto& physics = Component::physics[id];
            physics.onGround = on_ground;
        }

        void setRigidbody(EntityID id, bool is_rigidbody) noexcept
        {
            auto& physics = Component::physics[id];
            physics.isRigidbody = is_rigidbody;
        }

        bool isMidAir(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id];
            return !physics.onGround;
        }

        bool getOnGround(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id];
            return physics.onGround;
        }

        float getSpeed(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id];
            return physics.speed;
        }

        unsigned int getMaxJumpHeight(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id];
            return physics.maxJumpHeight;
        }

        bool getRigidbody(EntityID id) noexcept
        {
            if(Component::physics.find(id) != Component::physics.end())
            {
                const auto& physics = Component::physics[id];
                return physics.isRigidbody;
            }

            return false;
        }

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept
            {
                const sf::FloatRect idGlobalBounds  = System::Base::getSprite(id).getGlobalBounds();
                const sf::FloatRect sidGlobalBounds = System::Base::getSprite(second_id).getGlobalBounds();
            
                const float idRight   = idGlobalBounds.left  + (idGlobalBounds.width / 2);
                const float idBottom  = idGlobalBounds.top   + (idGlobalBounds.height / 2);
                const float sidRight  = sidGlobalBounds.left + (sidGlobalBounds.width / 2);
                const float sidBottom = sidGlobalBounds.top  + (sidGlobalBounds.height / 2);

                if(idGlobalBounds.intersects(sidGlobalBounds)) {
                    // Touching on the top
                    if(sidBottom >= idGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        return COLLISION::TOP;
                    }

                    // Touching on the right
                    if(idRight >= sidGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        return COLLISION::RIGHT;
                    }

                    // Touching on the left
                    if(sidRight >= idGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        return COLLISION::LEFT;
                    }

                    // Touching on the bottom
                    if(idBottom >= sidGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        return COLLISION::BOTTOM;
                    }
                }

                return COLLISION::NONE;
            }

            void checkFalling(EntityID id) noexcept
            {
                auto& physics  = Component::physics[id];
                
                // Falling when not touching anything or Jumping
                if(Physics::isMidAir(id) && not Movement::getJumping(id)) {
                    System::Base::getSprite(id).move(0, Physics::getSpeed(id));
                } 
                else if(Movement::getJumping(id)) 
                {
                    System::Base::getSprite(id).move(0, Physics::getSpeed(id) * -1);

                    if(sf::Time timer = physics.jumpClock.getElapsedTime();
                    timer >= sf::milliseconds(Physics::getMaxJumpHeight(id)))
                    {
                        Movement::setJumping(id, false);
                        physics.jumpClock.restart();
                    }
                }
            }

            void checkTouchingObject(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                const auto typeID       = Type::getType(id);
                const auto secondIDType = Type::getType(second_id);
                if(typeID == Enum::Type::MARIO)
                {
                    // Entities
                    if(secondIDType == Enum::Type::COIN) {
                        Physics::Helper::checkTouchedCoin(id, second_id, collision);
                    }
                    else if(secondIDType == Enum::Type::BLOCK) {
                        Physics::Helper::checkTouchedBlock(id, second_id, collision);
                    }
                    else if(secondIDType == Enum::Type::MUSHROOM) {
                        Physics::Helper::checkTouchedMushroom(id, second_id, collision);
                    }
                    else if(secondIDType == Enum::Type::FLOWER) {
                        Physics::Helper::checkTouchedFlower(id, second_id, collision);
                    }

                    // Enemies
                    else if(secondIDType == Enum::Type::GOOMBA) {
                        Physics::Helper::checkTouchedGoomba(id, second_id, collision);
                    }
                    else if(secondIDType == Enum::Type::SPINY) {
                        Physics::Helper::checkTouchedSpiny(id, second_id, collision);
                    }
                }
                else if(typeID == Enum::Type::FIRE) 
                {
                    if(secondIDType == Enum::Type::GOOMBA ||
                        secondIDType == Enum::Type::SPINY) 
                    {
                        Physics::Helper::fireTouchedEnemy(id, second_id, collision);
                    }
                }
            }

            bool checkTouchedCoin(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision != COLLISION::NONE) {
                    Game::removeID(second_id, WAIT_FOR_ANIM::FALSE);
                    return true;
                }

                return false;
            }

            bool checkTouchedBlock(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision == COLLISION::BOTTOM) {
                    Animation::setAllowPlay(second_id, true);
                    return true;
                }

                return false;
            }

            bool checkTouchedGoomba(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision == COLLISION::TOP) 
                {
                    System::Base::setState(second_id, Enum::State::DEAD);
                    Animation::setCurrentAnimation(second_id, int(Enum::Animation::DEAD));
                    Game::removeID(second_id, WAIT_FOR_ANIM::TRUE);
                    Movement::jump(id, PLAYER_KILL, FORCE::TRUE);
                    return true;
                }

                return false;
            }

            bool checkTouchedSpiny(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision != COLLISION::NONE) 
                {
                    return true;
                }

                return false;
            }

            bool checkTouchedMushroom(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision != COLLISION::NONE) 
                {
                    const auto mature = System::Type::getMaturity(id);
                    if(mature == Enum::Mature::CHILD) 
                    {
                        System::Type::setWhatType(id, Enum::Mature::TEENAGE);
                        Game::removeID(second_id, WAIT_FOR_ANIM::FALSE);

                        if(Physics::getOnGround(id)){   
                            Movement::jump(id, PLAYER_PICKED_MUSHROOM_ON_FLOOR, FORCE::FALSE);
                        }
                    } 
                    else {
                        Game::removeID(second_id, WAIT_FOR_ANIM::FALSE);
                    }

                    return true;
                }

                return false;
            }

            bool checkTouchedFlower(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision != COLLISION::NONE) 
                {
                    const auto mature = System::Type::getMaturity(id);
                    if(mature == Enum::Mature::TEENAGE) 
                    {
                        System::Type::setWhatType(id, Enum::Mature::ADULT);
                        Game::removeID(second_id, WAIT_FOR_ANIM::FALSE);
                    
                        if(Physics::isMidAir(id)) {
                            Movement::jump(id, PLAYER_PICKED_FLOWER_ON_FLOOR, FORCE::FALSE);
                        }
                    } 
                    else {
                        // Instead of writing what inside mushroom in here
                        checkTouchedMushroom(id, second_id, collision);
                    }

                    return true;
                }

                return false;
            }

            bool fireTouchedEnemy(EntityID id, EntityID second_id, COLLISION collision) noexcept
            {
                if(collision != COLLISION::NONE)
                {
                    System::Base::setState(second_id, Enum::State::DEAD);
                    Animation::setCurrentAnimation(second_id, int(Enum::Animation::DEAD));
                    Game::removeID(second_id, WAIT_FOR_ANIM::TRUE);
                    Game::removeID(id, WAIT_FOR_ANIM::FALSE);
                    
                    return true;
                }

                return false;
            }
        } // namespace Helper
    } // namespace Physics

    namespace GlobalVariables
    {
        sf::Clock& getClock(EntityID id) noexcept
        {
            auto& global = Component::globalVariables[id];

            if(not global.clock.has_value()) {
                global.clock = std::make_optional<sf::Clock>();
            }

            return global.clock.value();
        }

        bool existsAny(EntityID id, size_t vector_position) noexcept
        {
            auto& global = Component::globalVariables[id];
            return global.values[vector_position].has_value();
        }
    }
} // namespace System