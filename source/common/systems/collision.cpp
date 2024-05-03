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
    bool debug = true;
    struct BoundingBox
    {
        float minX, maxX;
        float minY, maxY;
        float minZ, maxZ;
    };

    bool checkCollision(const BoundingBox &box1, const BoundingBox &box2)
    {
        return (box1.minX < box2.maxX && box1.maxX > box2.minX) &&
               (box1.minY < box2.maxY && box1.maxY > box2.minY) &&
               (box1.minZ < box2.maxZ && box1.maxZ > box2.minZ);
    }

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
                              glm::vec4(playerEntity->localTransform.position, 1)); // get the player's position in the world
                break;
                // glm::vec3 &position = entity->localTransform.position;
            }
        }
        if (!player)
        {
            return false; // If the player doesn't exist, we can't do collision detection
        }
        glm::vec3 playerStart = playerEntity->getComponent<CollisionComponent>()->start + playerPosition; // get the player's start position
        glm::vec3 playerEnd = playerEntity->getComponent<CollisionComponent>()->end + playerPosition;     // get the player's end position
        BoundingBox playerBox = {playerStart[0], playerEnd[0], playerStart[1], playerEnd[1], playerStart[2], playerEnd[2]};
        if (debug)
        {
            std::cout << "player position " << playerPosition.x << " " << playerPosition.y << " " << playerPosition.z << std::endl;
            // Get player collision bounding box

            std::cout << "start player x " << playerStart.x << "  start player y " << playerStart.y << "   start player z " << playerStart.z << std::endl;
            std::cout << "end player x " << playerEnd.x << "  end player y " << playerEnd.y << "   end player z " << playerEnd.z << std::endl;
        }

        // For each entity in the world
        for (auto entity : world->getEntities())
        {
            // Get the collision component if it exists
            CollisionComponent *collision = entity->getComponent<CollisionComponent>();
            // If the collision component exists
            if (collision)
            {

                auto objectPosition = entity->localTransform.position; // get the object's position in the world
                // Get object collision bounding box
                glm::vec3 objectStart = (collision->start + objectPosition); // get the object's start position
                glm::vec3 objectEnd = (objectPosition + collision->end);     // get the object's end position

                BoundingBox obstacleBox = {objectStart[0], objectEnd[0], objectStart[1], objectEnd[1], objectStart[2], objectEnd[2]};
                collided = checkCollision(playerBox, obstacleBox);

                if (entity->getComponent<FenceComponent>() && entity->name == "left fence" && debug)
                {
                    std::cout << "start fence x " << objectStart.x << "  start fence y " << objectStart.y << "   start fence z " << objectStart.z << std::endl;
                    std::cout << "end fence x " << objectEnd.x << "  end fence y " << objectEnd.y << "   end fence z " << objectEnd.z << std::endl;
                }

                if (collided)
                {
                    if (entity->getComponent<PlayerComponent>())
                    {
                        // std::cout << "Collide with Player"<< std::endl;
                        continue;
                    }
                    // Player hits an obstacle
                    if (entity->getComponent<FenceComponent>())
                    { // if the object is an obstacle

                        if (this->lastFencePosition != entity->localTransform.position)
                        {
                            if (player->hearts <= 1)
                            {
                                app->changeState("game-over");
                            }
                            else
                            {
                                player->hearts = player->hearts - 1;
                                this->lastFencePosition = entity->localTransform.position;
                            }
                        }

                        std::cout << "my hearts =" << player->hearts << std::endl;
                        std::cout << "collided with " << entity->name << " " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                    }
                    else if (entity->getComponent<TrainComponent>())
                    {
                        if (player->hearts <= 1)
                        {
                            app->changeState("game-over");
                        }
                        player->hearts = player->hearts - 1;
                        std::cout << "my hearts =" << player->hearts << std::endl;
                        std::cout << "collided with obstacle : TrainComponent " << std::endl;
                    }
                    if (entity->getComponent<CoinComponent>())
                    { // if the object is an obstacle

                        if (this->lastCoinPosition != entity->localTransform.position)
                        {
                            player->score = player->score + 10;
                            this->lastCoinPosition = entity->localTransform.position;
                        }
                        std::cout << "my score =" << player->score << std::endl;
                        std::cout << "collided with " << entity->name << " " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                    }
                    else
                    {
                        std::cout << "other Obstacle " << std::endl;
                    }
                }
                else if (debug)
                {
                    std::cout << entity->name << std::endl;
                    std::cout << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                }
            }
        }
        return collided;
    }
}