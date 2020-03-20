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
        if(Manager::canAccess(id)) 
        {
            auto& base = Component::bases[id];
            m_window.draw(base.sprite);
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
                    auto& base = Component::bases[id];
                    base.sprite.setTextureRect(animation.animations[pos][0]);
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
            auto& base = Component::bases[id];

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
                    base.sprite.setTextureRect(animation.animations[animation.currentAnimation][animation.currentFrame]);

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

        const AnimationVector* getFrames(EntityID id, int pos) noexcept
        {
            const auto& animation = Component::animations[id];
            if(animation.animations.find(pos) != animation.animations.end()) { 
                return &animation.animations.at(pos);
            }

            return nullptr;
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
            auto& base = Component::bases[id];
            base.sprite.move(sf::Vector2f(speed, 0));
            
            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id];
            base.sprite.move(sf::Vector2f(speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveLeft(EntityID id, float speed) noexcept
        {
            auto& base = Component::bases[id];
            base.sprite.move(sf::Vector2f(-speed, 0));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);
        }

        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id];
            base.sprite.move(sf::Vector2f(-speed, 0));

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
            auto& physics  = Component::physics[id];
            auto& base     = Component::bases[id];
            
            bool touchingGround = false;
            Enum::Direction blockedDirection = Enum::Direction::NONE;

            for(EntityID secondID = 0; secondID < Component::maxIndexes; secondID++) 
            {
                if(Manager::canAccess(secondID)) 
                {
                    if(secondID != id) 
                    {
                        COLLISION collision = Physics::Helper::checkIntersections(id, secondID);
                        auto& secondIDType = Component::types[secondID];

                        if(secondIDType.type == Enum::Type::BLOCK)
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

                        const auto& typeID = Component::types[id].type;
                        if(typeID == Enum::Type::MARIO)
                        {
                            if(secondIDType.type == Enum::Type::COIN) 
                            {
                                if(collision != COLLISION::NONE) {
                                    Game::removeID(secondID, WAIT_FOR_ANIM::FALSE);
                                }
                            } 
                            else if(secondIDType.type == Enum::Type::BLOCK) 
                            {
                                if(collision == COLLISION::BOTTOM) {
                                    Animation::setAllowPlay(secondID, true);
                                }
                            } 
                            else if(secondIDType.type == Enum::Type::GOOMBA) 
                            {
                                if(collision == COLLISION::TOP) 
                                {
                                    Animation::setCurrentAnimation(secondID, int(Enum::Animation::DEAD));
                                    Game::removeID(secondID, WAIT_FOR_ANIM::TRUE);
                                    Movement::jump(id, PLAYER_KILL, FORCE::TRUE);
                                }
                            } 
                            else if(secondIDType.type == Enum::Type::MUSHROOM)
                            {
                                if(collision != COLLISION::NONE) {
                                    auto& mature = std::get<Enum::Mature>(*Component::types[id].whatType);
                                    if(mature == Enum::Mature::CHILD) {
                                        mature = Enum::Mature::TEENAGE;
                                        Game::removeID(secondID, WAIT_FOR_ANIM::FALSE);
                                    }
                                }
                            }
                        }
                        else if(typeID == Enum::Type::FIRE) 
                        {
                            if(secondIDType.type == Enum::Type::GOOMBA) 
                            {
                                if(collision != COLLISION::NONE)
                                {
                                    Animation::setCurrentAnimation(secondID, int(Enum::Animation::DEAD));
                                    Game::removeID(secondID, WAIT_FOR_ANIM::TRUE);
                                    Game::removeID(id, WAIT_FOR_ANIM::FALSE);
                                }
                            }
                        }
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

            // Falling when not touching anything or Jumping
            if(Physics::isMidAir(id) && not Movement::getJumping(id)) {
                base.sprite.move(0, Physics::getSpeed(id));
            } 
            else if(Movement::getJumping(id)) 
            {
                base.sprite.move(0, Physics::getSpeed(id) * -1);

                if(sf::Time timer = physics.jumpClock.getElapsedTime();
                   timer >= sf::milliseconds(Physics::getMaxJumpHeight(id)))
                {
                    Movement::setJumping(id, false);
                    physics.jumpClock.restart();
                }
            }
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

        void setOnGround(EntityID id, bool on_ground) noexcept
        {
            auto& physics = Component::physics[id];
            physics.onGround = on_ground;
        }

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept
            {
                const sf::FloatRect idGlobalBounds  = Component::bases[id].sprite.getGlobalBounds();
                const sf::FloatRect sidGlobalBounds = Component::bases[second_id].sprite.getGlobalBounds();
            
                const float idRight   = idGlobalBounds.left  + (idGlobalBounds.width / 2);
                const float idBottom  = idGlobalBounds.top   + (idGlobalBounds.height / 2);
                const float sidRight  = sidGlobalBounds.left + (sidGlobalBounds.width / 2);
                const float sidBottom = sidGlobalBounds.top  + (sidGlobalBounds.height / 2);

                if(idGlobalBounds.intersects(sidGlobalBounds)) {
                    std::vector<COLLISION> collisionArray;
                    // only 3 sides possible to touch
                    collisionArray.reserve(3); 
                    
                    // Touching on the top
                    if(sidBottom >= idGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        collisionArray.push_back(COLLISION::TOP);
                    }

                    // Touching on the right
                    if(idRight >= sidGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        collisionArray.push_back(COLLISION::RIGHT);
                    }

                    // Touching on the left
                    if(sidRight >= idGlobalBounds.left && idBottom >= sidGlobalBounds.top && sidBottom >= idGlobalBounds.top) {
                        collisionArray.push_back(COLLISION::LEFT);
                    }

                    // Touching on the bottom
                    if(idBottom >= sidGlobalBounds.top && idRight >= sidGlobalBounds.left && sidRight >= idGlobalBounds.left) {
                        collisionArray.push_back(COLLISION::BOTTOM);
                    }

                    // if hit multiple sides, return top
                    if(collisionArray.size() > 1) {
                        return COLLISION::TOP;
                    } else if(collisionArray.size() != 0) {
                        return collisionArray.at(0); 
                    }
                }

                return COLLISION::NONE;
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
    }
} // namespace System