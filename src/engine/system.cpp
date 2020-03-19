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
            auto& base = Component::bases[id].value();
            m_window.draw(base.sprite);
        }
    }

    void Render::drawAll() noexcept 
    {
        for(size_t i = 0; i < Component::maxIndexes; i++) {
            Render::draw(i);
        }
    }

    // ----------- Game Loop ------------ //
    namespace Game
    {
        void update(EntityID id) noexcept 
        {
            if(Manager::canAccess(id)) 
            {
                auto& update = Component::updates[id].value();
                update(id);
            }
        }

        void updateAll() noexcept 
        {
            for(size_t i = 0; i < Component::updates.size(); i++) {
                Game::update(i);
            }

            /* first  = EntityID
               second = waiting for animation*/
            for(auto& i : Helper::removeableIDS) 
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

        namespace Helper 
        {
            void removeID(EntityID id, WAIT_FOR_ANIM wait_for_anim) noexcept {
                removeableIDS.push_back( std::make_pair(id, bool(wait_for_anim)) );
            }
        } // namespace Helper
    } // namespace Game


    // ----------- Animation ------------ //
    namespace Animation 
    {
        void setFrames(EntityID id, int pos, const AnimationVector& anims) noexcept 
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos] = anims;
        }

        void setFrames(EntityID id, int pos, const sf::IntRect& anim) noexcept 
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos] = { anim };
        }

        void addFrame(EntityID id, int pos, const sf::IntRect& anim) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.animations[pos].push_back(anim);
        }

        void setCurrentAnimation(EntityID id, int pos) noexcept 
        {
            auto& animation = Component::animations[id].value();
            
            // make sure position exists
            if(animation.animations.find(pos) != animation.animations.end()) 
            { 
                if(pos != animation.currentAnimation) 
                {
                    Animation::setStarted(id, false);
                    Animation::setFinished(id, false);

                    animation.currentAnimation = pos;

                    // Set the first frame to sprite
                    auto& base = Component::bases[id].value();
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
            auto& animation = Component::animations[id].value();
            animation.nextFrameTimer = next_animation_timer;
        }

        void setStopWhenFinished(EntityID id, bool stop) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.stopWhenFinished = stop;
        }

        void setAllowPlay(EntityID id, bool allow) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.allowPlay = allow;
        }

        void setStarted(EntityID id, bool started) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.isStarted = started;
        }

        void setFinished(EntityID id, bool finished) noexcept
        {
            auto& animation = Component::animations[id].value();
            animation.isFinished = finished;
        }

        void play(EntityID id) noexcept 
        {
            auto& animation = Component::animations[id].value();
            auto& base = Component::bases[id].value();

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
            const auto& animation = Component::animations[id].value();
            return animation.isFinished;
        }

        const AnimationVector* getFrames(EntityID id, int pos) noexcept
        {
            const auto& animation = Component::animations[id].value();
            if(animation.animations.find(pos) != animation.animations.end()) { 
                return &animation.animations.at(pos);
            }

            return nullptr;
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
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(speed, 0));
            
            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveRight(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::RIGHT);
        }

        void moveLeft(EntityID id, float speed) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(-speed, 0));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);
        }

        void moveLeft(EntityID id, float speed, Enum::Animation anim) noexcept
        {
            auto& base = Component::bases[id].value();
            base.sprite.move(sf::Vector2f(-speed, 0));

            Animation::setCurrentAnimation(id, int(anim));

            Movement::setMoving(id, true);
            Movement::setLookingDirection(id, Enum::Direction::LEFT);
        }


        void jump(EntityID id, unsigned int height, FORCE force)
        {
            #ifdef ENABLE_DEBUG_MODE
            if(not Component::physics[id].has_value()) {
                throw std::runtime_error("Please add physics component to enable jumping!");
            }
            #endif

            auto& movement = Component::movements[id].value();
            if((not movement.isJumping && Physics::getOnGround(id)) || bool(force)) 
            {
                Movement::setJumping(id, true);

                auto& physics = Component::physics[id].value();
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }

        void jump(EntityID id, unsigned int height, Enum::Animation anim, FORCE force)
        {
            #ifdef ENABLE_DEBUG_MODE
            if(not Component::physics[id].has_value()) {
                throw std::runtime_error("Please add physics component to enable jumping!");
            }
            #endif

            auto& movement = Component::movements[id].value();
            if((not movement.isJumping && Physics::getOnGround(id)) || bool(force)) 
            {
                Movement::setJumping(id, true);
                
                auto& physics = Component::physics[id].value();
                physics.maxJumpHeight = height;
                physics.jumpClock.restart();
            }
        }

        void setLookingDirection(EntityID id, Enum::Direction direction) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.lookingDirection = direction;
        }

        void setBlockedDirection(EntityID id, Enum::Direction direction) noexcept 
        {
            auto& movement = Component::movements[id].value();
            movement.blockedDirection = direction;
        }

        void setMoving(EntityID id, bool moving) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isMoving = moving;
        }

        void setRunning(EntityID id, bool running) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isRunning = running;
        }

        void setJumping(EntityID id, bool jumping) noexcept
        {
            auto& movement = Component::movements[id].value();
            movement.isJumping = jumping;
        }

        Enum::Direction getLookingDirection(EntityID id) noexcept 
        {
            const auto& movement = Component::movements[id].value();
            return movement.lookingDirection;
        }

        Enum::Direction getBlockedDirection(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id].value();
            return movement.blockedDirection;
        }

        bool getJumping(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id].value();
            return movement.isJumping;
        }

        bool getRunning(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id].value();
            return movement.isRunning;
        }

        bool getMoving(EntityID id) noexcept
        {
            const auto& movement = Component::movements[id].value();
            return movement.isMoving;
        }
    } // namespace Movement

    // ----------- Physics ------------ //
    namespace Physics
    {
        void start(EntityID id) noexcept
        {
            auto& physics  = Component::physics[id].value();
            
            bool touchingGround = false;
            Enum::Direction blockedDirection = Enum::Direction::NONE;

            // auto& playerMaturity = std::get<Enum::Mature>(Component::types[id] ->whatType.value());

            for(EntityID secondID = 0; secondID < Component::maxIndexes; secondID++) 
            {
                if(Manager::canAccess(secondID)) 
                {
                    if(secondID != id) 
                    {
                        COLLISION collision = Physics::Helper::checkIntersections(id, secondID);
                        #ifdef ENABLE_DEBUG_MODE
                        std::cout << "ID:" << secondID << " Collision:" << int(collision) << std::endl;
                        #endif
                        auto& secondIDType = Component::types[secondID].value();

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

                        if(/*is player*/Component::types[id].value().type == Enum::Type::MARIO)
                        {
                            auto& type = Component::types[secondID].value().type;
                            if(type == Enum::Type::COIN) {
                                Physics::Helper::checkTouchedCoin(secondID, collision);
                            } else if(type == Enum::Type::BLOCK) {
                                Physics::Helper::checkTouchedBlock(secondID, collision);
                            } else if(type == Enum::Type::GOOMBA) {
                                Physics::Helper::checkTouchedGoomba(id, secondID, collision);
                            }
                        }
                    }
                }
            }

            if(touchingGround) {
                Physics::setOnGround(id, true);
            } else {
                Physics::setOnGround(id, false);
            }

            Movement::setBlockedDirection(id, blockedDirection);

            if(Physics::isMidAir(id) && not Movement::getJumping(id)) {
                Component::bases[id] ->sprite.move(0, Physics::getSpeed(id));
            } else if(Movement::getJumping(id)) {
                #ifdef ENABLE_DEBUG_MODE
                std::cout << "ID:" << id << " is jumping!";
                #endif

                Component::bases[id] ->sprite.move(0, Physics::getSpeed(id) * -1);

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
            const auto& physics = Component::physics[id].value();
            return !physics.onGround;
        }

        bool getOnGround(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id].value();
            return physics.onGround;
        }

        float getSpeed(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id].value();
            return physics.speed;
        }

        unsigned int getMaxJumpHeight(EntityID id) noexcept
        {
            const auto& physics = Component::physics[id].value();
            return physics.maxJumpHeight;
        }

        void setOnGround(EntityID id, bool on_ground) noexcept
        {
            auto& physics = Component::physics[id].value();
            physics.onGround = on_ground;
        }

        namespace Helper
        {
            COLLISION checkIntersections(EntityID id, EntityID second_id) noexcept
            {
                const sf::FloatRect idGlobalBounds  = Component::bases[id]        -> sprite.getGlobalBounds();
                const sf::FloatRect sidGlobalBounds = Component::bases[second_id] -> sprite.getGlobalBounds();
            
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

            void checkTouchedCoin(EntityID second_id, COLLISION collision)
            {
                if(collision != COLLISION::NONE) {
                    Game::Helper::removeID(second_id, WAIT_FOR_ANIM::FALSE);
                }
            }

            void checkTouchedBlock(EntityID second_id, COLLISION collision)
            {
                if(collision == COLLISION::BOTTOM) {
                    Animation::setAllowPlay(second_id, true);
                }
            }

            void checkTouchedGoomba(EntityID id, EntityID second_id, COLLISION collision)
            {
                if(collision == COLLISION::TOP) 
                {
                    Animation::setCurrentAnimation(second_id, int(Enum::Animation::DEAD));
                    Game::Helper::removeID(second_id, WAIT_FOR_ANIM::TRUE);
                    Movement::jump(id, PLAYER_KILL, FORCE::TRUE);
                }
            }
        } // namespace Helper
        
    } // namespace Physics

    // ----------- Globals ------------ //
    namespace Global
    {        
        void setMoveLeft(EntityID id, bool move_left) noexcept
        {
            auto& global = Component::globals[id].value();
            global.moveLeft = move_left;
        }

        bool setLeftLookingDirectionOnce(EntityID id, bool looking_direction) noexcept
        {
            auto& global = Component::globals[id].value();
            if(not global.lookingDirection.has_value()) {
                global.lookingDirection = looking_direction;
                return true;
            }

            return false;
        }

        sf::Clock& getClock(EntityID id) noexcept
        {
            auto& global = Component::globals[id].value();
            return global.clock;
        }

        bool getMoveLeft(EntityID id) noexcept
        {
            const auto& global = Component::globals[id].value();
            return global.moveLeft.has_value() ? global.moveLeft.value() : true;
        }

        const bool* getLeftLookingDirection(EntityID id) noexcept
        {
            const auto& global = Component::globals[id].value();
            return global.lookingDirection.has_value() ? &global.lookingDirection.value() : nullptr;
        }

    } // namespace Globals

} // namespace System