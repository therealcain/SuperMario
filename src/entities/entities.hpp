#ifndef ENTITIES_HPP
#define ENTITIES_HPP

#include <SFML/Graphics.hpp>
#include "../engine/components.hpp"
#include "../engine/manager.hpp"

namespace Entity
{
    namespace Block 
    {
        void create(const sf::Vector2f& position, Enum::Block block_type, std::optional<Enum::Type> type) noexcept;

        namespace Helper
        {
            void errorCheck(Enum::Block block_type, std::optional<Enum::Type> type);
            void setupAnimations(EntityID id, Enum::Block type) noexcept;
            void setupUpdateFunction(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Block

    namespace Cloud 
    {
        void create(const sf::Vector2f& position) noexcept;
    } // namespace Cloud

    namespace Coin 
    {
        void create(const sf::Vector2f& position) noexcept;

        namespace Helper
        {
            void setupAnimations(EntityID id) noexcept;
            void setupUpdateFunction(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Coin

    namespace Mushroom
    {
        void create(const sf::Vector2f& position) noexcept;

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept;
            void jumpOutOnce(EntityID id) noexcept;
            void checkBlockedDirections(EntityID id) noexcept;
            void movement(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Mushroom

    namespace Flower
    {
        void create(const sf::Vector2f& position) noexcept;

        namespace Helper
        {
            void setupUpdateFunction(EntityID id) noexcept;
            void jumpOutOnce(EntityID id) noexcept;
        } // namespace Helper
    } // namespace Flower
} // namespace Entity

#endif