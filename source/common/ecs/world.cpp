#include "world.hpp"
#include "../common/components/movement.hpp"
#include "../common/components/player.hpp"
#include <iostream>

namespace our
{

    // This will deserialize a json array of entities and add the new entities to the current world
    // If parent pointer is not null, the new entities will be have their parent set to that given pointer
    // If any of the entities has children, this function will be called recursively for these children
    void World::deserialize(const nlohmann::json &data, Entity *parent)
    {
        if (!data.is_array())
            return;
        for (const auto &entityData : data)
        {
            // TODO: (Req 8) Create an entity, make its parent "parent" and call its deserialize with "entityData".
            Entity *element = this->add();
            element->parent = parent;
            element->deserialize(entityData);
            if (element->getComponent<MovementComponent>() && element->getComponent<PlayerComponent>())
            {
                MovementComponent *move = element->getComponent<MovementComponent>();
                move->linearVelocity = glm::vec3(0, 0, -5 *2* this->level);
            }

            if (element->getComponent<CameraComponent>() && element->getComponent<FreeCameraControllerComponent>())
            {
                MovementComponent *move = element->getComponent<MovementComponent>();
                move->linearVelocity = glm::vec3(0, 0, -5 *2* this->level);
            }
            if (entityData.contains("children"))
            {
                // TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                nlohmann::json children_data = entityData["children"];
                World::deserialize(children_data, element);
            }
        }
    }

}