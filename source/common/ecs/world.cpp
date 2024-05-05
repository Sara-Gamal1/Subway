#include "world.hpp"
#include "../common/components/movement.hpp"
#include "../common/components/player.hpp"
#include <iostream>
#include <vector>
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
                move->linearVelocity = glm::vec3(0, 0, -5 * 2 * this->level);
            }

            if (element->getComponent<CameraComponent>() && element->getComponent<FreeCameraControllerComponent>())
            {
                MovementComponent *move = element->getComponent<MovementComponent>();
                move->linearVelocity = glm::vec3(0, 0, -5 * 2 * this->level);
            }
            if (entityData.contains("children"))
            {
                // TODO: (Req 8) Recursively call this world's "deserialize" using the children data
                //  and the current entity as the parent
                nlohmann::json children_data = entityData["children"];
                World::deserialize(children_data, element);
            }
            if (entityData.contains("duplicates"))
            {
                // glm::vec3 duplicates = glm::vec3(entityData.value("duplicates", duplicates));
                nlohmann::json duplicates = entityData["duplicates"];
                for (int i = 1; i <= (int)duplicates[0]; ++i)
                {
                    Entity *newDuplicateEntity = add();  // create a new entity using the add function in world.hpp
                    newDuplicateEntity->parent = parent; // set the parent of the new entity to the given parent
                    newDuplicateEntity->deserialize(     // deserialize the new entity using the given entityData
                    entityData);
                    newDuplicateEntity->localTransform.position.x += int(i) * (int)duplicates[1];
                }

                for (int i = 1; i <= (int)duplicates[2]; ++i)
                {
                    Entity *newDuplicateEntity = add();  // create a new entity using the add function in world.hpp
                    newDuplicateEntity->parent = parent; // set the parent of the new entity to the given parent
                    newDuplicateEntity->deserialize(     // deserialize the new entity using the given entityData
                    entityData);
                    newDuplicateEntity->localTransform.position.y += int(i) * (int)duplicates[3];
                }

                for (int i = 1; i <= (int)duplicates[4]; ++i)
                {
                    Entity *newDuplicateEntity = add();  // create a new entity using the add function in world.hpp
                    newDuplicateEntity->parent = parent; // set the parent of the new entity to the given parent
                    newDuplicateEntity->deserialize(     // deserialize the new entity using the given entityData
                    entityData);
                    newDuplicateEntity->localTransform.position.z += int(i) * (int)duplicates[5];
                }
            }
        }
    }
}