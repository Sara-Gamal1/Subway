// TODO: (Phase 2)

#include "collision.hpp"

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/coin.hpp"
#include "../components/train.hpp"
#include "../components/fence.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include <iostream>

#include "../mesh/mesh-utils.hpp"
#include "../texture/texture-utils.hpp"

#include "../application.hpp"

namespace our
{
    struct BoundingBox
    {
        float minX, maxX;
        float minY, maxY;
        float minZ, maxZ;
    };

    float absolute(float value)
    {
        return value >= 0 ? value : -value;
    }
    bool checkCollision(const BoundingBox &box1, const BoundingBox &box2)
    {
        return (box1.minX < box2.maxX && box1.maxX > box2.minX) &&
               (box1.minY < box2.maxY && box1.maxY > box2.minY) &&
               (box1.minZ - 3 < box2.maxZ && box1.maxZ - 3 > box2.minZ);
    }

    // void CollisionSystem::checkMasalaHeight(World *world, float deltaTime)
    // {
    //     // Entity *tempEntity = nullptr;
    //     MasalaComponent *masala = nullptr;
    //     for (auto entity : world->getEntities())
    //     { // search for the player entity
    //         // Get the MASALA component if it exists
    //         // playerEntity = entity;
    //         masala = entity->getComponent<MasalaComponent>();
    //         // If the player component exists
    //         if (masala)
    //         {
    //             if (masala->increasingHeight)
    //             {
    //                 // glm::vec3 masalaCoordinates = glm::vec3(entity->getLocalToWorldMatrix() *
    //                 //             glm::vec4(entity->localTransform.position, 1.0)); // get the player's position in the world

    //                 glm::vec3 &position = entity->localTransform.position;
    //                 position.y += 1.2 * deltaTime;
    //                 if (position.y > -3)
    //                 {
    //                     masala->increasingHeight = false;
    //                 }
    //             }
    //             else
    //             {
    //                 glm::vec3 &position = entity->localTransform.position;
    //                 position.y -= 0.6 * deltaTime;
    //                 if (position.y < -10)
    //                 {
    //                     masala->increasingHeight = true;
    //                 }
    //             }
    //         }
    //     }
    // }

    bool CollisionSystem::update(World *world, float deltaTime, int heartCount,
                                 float collisionStartTime)
    {
        // checkMasalaHeight(world, 0.05);
        PlayerComponent *player;  // The player component if it exists
        glm::vec3 playerPosition; // The player's position in the world
        Entity *playerEntity;     // The player entity if it exists
        bool collided = false;
        for (auto entity : world->getEntities())
        {
            // search for the player entity
            // Get the player component if it exists
            playerEntity = entity;
            player = playerEntity->getComponent<PlayerComponent>();
            // If the player component exists
            if (player)
            {
                playerPosition =
                    glm::vec3(playerEntity->getLocalToWorldMatrix() *
                              glm::vec4(playerEntity->localTransform.position, 1.0)); // get the player's position in the world
                break;
                // glm::vec3 &position = entity->localTransform.position;
            }
        }
        if (!player)
        {
            return false; // If the player doesn't exist, we can't do collision detection
        }

        // // Get player collision bounding box
        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition; // get the player's start position
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;     // get the player's end position
        BoundingBox playerBox = {playerStart[0], playerEnd[0], playerStart[1], playerEnd[1], playerStart[2], playerEnd[2]};

        // For each entity in the world
        for (auto entity : world->getEntities())
        {
            // Get the collision component if it exists
            // std::string type = data.value("type", "");
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision)
            {

                auto objectPosition = entity->localTransform.position; // get the object's position in the world
                auto objectScale = entity->localTransform.scale;       // get the object's scale

                // Get object collision bounding box
                glm::vec3 objectStart = (collision->start + objectPosition); // get the object's start position
                glm::vec3 objectEnd = (objectPosition + collision->end);     // get the object's end position

                BoundingBox obstacleBox = {objectStart[0], objectEnd[0], objectStart[1] - 1, objectEnd[1] - 1, objectStart[2], objectEnd[2]};
                collided = checkCollision(playerBox, obstacleBox);

                if (collided)
                {
                    // TODO: remeber to remove it
                    if (entity->getComponent<PlayerComponent>())
                    {
                        // std::cout << "Collide with Player"<< std::endl;
                        continue;
                    }

                    // Player hits an obstacle
                    if (entity->getComponent<FenceComponent>())
                    { // if the object is an obstacle
                        std::cout << "collided with obstacle : FenceComponent " << std::endl;
                        // if (collisionStartTime == 0)
                        //     collisionStartTime = deltaTime; // start counting the time of collision for postprocessing effect
                        // else if (glfwGetTime() - collisionStartTime >= 1)
                        // {
                        //     continue;
                        // }
                        // CollisionSystem::decreaseHearts(world, heartCount);
                        // if (heartCount < 1)
                        // {
                        //     // if the player has no more hearts
                        //     heartCount = 3;                // reset the heart count
                        //     app->changeState("game-over"); // go to the game over state
                        // }
                    }
                    else if (entity->getComponent<TrainComponent>())
                    {
                        std::cout << "collided with obstacle : TrainComponent " << std::endl;
                    }
                    else
                    {
                        std::cout << "other Obstacle " << std::endl;
                    }
                    // // Player takes a heart
                    // else if (entity->getComponent<GemHeartComponent>()) // if the object is a gem heart
                    // {
                    //     if (heartCount < 3) // if the player has less than 3 hearts which is max
                    //     {
                    //         heartCount++; // increase the count of hearts
                    //     }
                    //     // Make heart to disappear
                    //     // entity->localTransform.scale = glm::vec3(0.0f, 0.0f, 0.0f);
                    //     // entity->localTransform.position = glm::vec3(0.0f, 0.0f, 0.0f);
                    //     for (auto heartEntity : world->getEntities())
                    //     {                                                                        // search for the heart entity
                    //         HeartComponent *heart = heartEntity->getComponent<HeartComponent>(); // get the heart component if it exists
                    //         if (heart && heart->heartNumber == heartCount)
                    //         {                                                // if the heart component exists and it's the heart that we want to increase
                    //             heartEntity->localTransform.scale.x = 0.05f; // make the heart appear
                    //             heartEntity->localTransform.scale.y = 0.05f; // make the heart appear
                    //             heartEntity->localTransform.scale.z = 0.05f; // make the heart appear
                    //             break;
                    //         }
                    //     }
                    // }

                    // RepeatComponent *repeatComponent = entity->getComponent<RepeatComponent>();
                    // glm::vec3 &repeatPosition = entity->localTransform.position;
                    // if (repeatComponent)
                    // {                                                   // if the object is a repeat object
                    //     repeatPosition += repeatComponent->translation; // move the object forward
                    // }
                    // break;
                }
            }
        }
        return (collided);
    }

    // decrease the hearts
    // void CollisionSystem::decreaseHearts(World *world, int &heartCount)
    // {
    //     for (auto heartEntity : world->getEntities())
    //     { // search for the heart entity
    //         HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
    //         if (heart && heart->heartNumber == heartCount)
    //         { // if the heart component exists and it's the heart that we want to decrease
    //             // make the heart disappear
    //             heartEntity->localTransform.scale.x = 0.0f;
    //             heartEntity->localTransform.scale.y = 0.0f;
    //             heartEntity->localTransform.scale.z = 0.0f;
    //             heartCount--; // decrease the count of hearts
    //             break;
    //         }
    //     }
    // }
}