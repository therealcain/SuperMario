#include "coin.hpp"

#include "../engine/manager.hpp"
#include "../engine/system.hpp"

namespace Entity {
    namespace Coin {
        void create(sf::Vector2f&& position) noexcept
        {   
            EntityID currentID = Manager::create("assets/coin.png");

            Component::bases[currentID] -> sprite.setPosition(position);
            Component::types[currentID] -> type = Enum::Type::COIN;

            Manager::addComponent<Component::Animation>(currentID, BE_NULL::FALSE);
            if(System::Animation::isPrepared(currentID)) {
                constexpr int animPos = 0;
                
                System::Animation::setFrames(currentID, animPos, {
                    sf::IntRect(0, 0, 10, 14),
                    sf::IntRect(16, 0, 10, 14),
                    sf::IntRect(32, 0, 10, 14),
                    sf::IntRect(48, 0, 10, 14),
                    sf::IntRect(3, 15, 3, 15),
                    sf::IntRect(17, 15, 7, 15),
                    sf::IntRect(35, 15, 4, 16),
                    sf::IntRect(53, 15, 1, 16)
                });

                System::Animation::setNextAnimationTimer(currentID, 100);
                System::Animation::setCurrentAnimation(currentID, animPos);
            }

            Manager::addComponent<Component::UpdateFunction>(currentID, BE_NULL::FALSE);
            Component::updates[currentID] = [](EntityID id) {
                System::Animation::play(id);
            };
        }

        inline void hit(EntityID id) noexcept
        {
            Manager::remove(id);
        }
    } // namespace Coin
} // namespace Entity