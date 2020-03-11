#include "enemies.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"
#include "../engine/helpers/functions.hpp"
#include "../engine/helpers/values.hpp"
#include "../engine/components.hpp"

namespace Enemy
{
    namespace Goomba 
    {
        void create(sf::Vector2f&& position) noexcept
        {  
            EntityID currentID = Manager::create("assets/goomba.png");

            Component::bases[currentID] ->sprite.setPosition(position);
            Component::types[currentID] ->type     = Enum::Type::GOOMBA;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) 
            {
                System::Animation::setFrames(currentID, int(Enum::Animation::WALK), {
                    System::Animation::Helper::extractTextureRect(sf::IntRect(0, 0, 16, 16)),
		            System::Animation::Helper::extractTextureRect(sf::IntRect(16, 0, 32, 16))
                });

                System::Animation::setFrames(currentID, int(Enum::Animation::DEAD), 
                    System::Animation::Helper::extractTextureRect(sf::IntRect(32, 8, 48, 16)));

                System::Animation::setCurrentAnimation(currentID, int(Enum::Animation::WALK));
            }  

            Manager::addComponent<Component::Movement>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::Physics>(currentID, BE_NULL::FALSE);
            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);

            Component::updates[currentID] = [](EntityID id) -> void 
            {
                if(System::Physics::isPrepared(id)) {
                    System::Physics::start(id);
                }

                if(System::Movement::isPrepared(id))
                {
                    System::Movement::moveLeft(id, ENEMY_SPEED);
                }

                System::Animation::play(id);
            };
        }
    } // namespace Goomba
} // namespace Enemy