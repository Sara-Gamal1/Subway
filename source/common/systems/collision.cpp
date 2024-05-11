// TODO: (Phase 2)

#include "collision.hpp"

#include "../ecs/world.hpp"
#include "../components/collision.hpp"
#include "../components/player.hpp"
#include "../components/coin.hpp"
#include "../components/police.hpp"
#include "../components/train.hpp"
#include "../components/fence.hpp"
#include "../components/end-line.hpp"
#include "../components/heart.hpp"
#include "../components/camera.hpp"
#include "../components/key.hpp"

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
    bool debug = false;
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
                                 float collisionStartTime, int &playerScore)
    {
        PlayerComponent *player;  // The player component if it exists
        glm::vec3 playerPosition; // The player's position in the world
        Entity *playerEntity;     // The player entity if it exists
        PoliceComponent *police;  // The player component if it exists
        glm::vec3 policePosition; // The player's position in the world
        Entity *policeEntity;     // The player entity if it exists
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

        for (auto entity : world->getEntities())
        {
            // search for the player entity
            // Get the player component if it exists
            policeEntity = entity;
            police = policeEntity->getComponent<PoliceComponent>();
            // If the player component exists
            if (police)
            {
                policePosition =
                    glm::vec3(policeEntity->getLocalToWorldMatrix() *
                              glm::vec4(policeEntity->localTransform.position, 1)); // get the player's position in the world
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

            std::cout << "start player x " << playerStart.x << "  start player y " << playerStart.y << "   start player z " << playerStart.z << std::endl;
            std::cout << "end player x " << playerEnd.x << "  end player y " << playerEnd.y << "   end player z " << playerEnd.z << std::endl;
        }
        CameraComponent *camera;
        for (auto entity : world->getEntities())
        {
            camera = entity->getComponent<CameraComponent>();
            if (camera)
                break;
        }

        // For each entity in the world
        lastCoinPosition = glm::vec3(-1, -1, -1);
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

                        // std::cout << "my hearts =" << player->hearts << std::endl;
                        // std::cout << "collided with " << entity->name << " " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                        if (player->hearts <= 1)
                        {
                            app->changeState("game-over");
                            playerScore = 0;
                        }
                        else
                        {
                            if (camera)
                            {
                                Entity *entity = camera->getOwner();
                                glm::vec3 &position = entity->localTransform.position;
                                lastRemovedheart = player->hearts;
                                decreaseHearts(world, player->hearts);
                                player->hearts = player->hearts - 1;

                                position.x = 0;
                                position.y = 5;
                                position.z = 50;
                                playerEntity->localTransform.position.x = 0;
                                playerEntity->localTransform.position.y = -5;
                                playerEntity->localTransform.position.z = 30;

                                policeEntity->localTransform.position.x = 0;
                                policeEntity->localTransform.position.y = -5;
                                policeEntity->localTransform.position.z = 32;
                            }
                        }
                        return true;
                    }
                    if (entity->getComponent<TrainComponent>())
                    {

                        // std::cout << "my hearts =" << player->hearts << std::endl;
                        // std::cout << "collided with obstacle : TrainComponent " << std::endl;
                        if (player->hearts <= 1)
                        {

                            app->changeState("game-over");
                        }
                        else
                        {

                            if (camera)
                            {
                                Entity *entity = camera->getOwner();
                                glm::vec3 &position = entity->localTransform.position;

                                lastRemovedheart = player->hearts;
                                decreaseHearts(world, player->hearts);
                                player->hearts = player->hearts - 1;

                                position.x = 0;
                                position.y = 5;
                                position.z = 50;
                                playerEntity->localTransform.position.x = 0;
                                playerEntity->localTransform.position.y = -5;
                                playerEntity->localTransform.position.z = 30;

                                policeEntity->localTransform.position.x = 0;
                                policeEntity->localTransform.position.y = -5;
                                policeEntity->localTransform.position.z = 32;
                            }
                        }
                        return true;
                    }
                    if (entity->getComponent<CoinComponent>())
                    {
                        // if the object is an obstacle
                        if (this->lastCoinPosition != entity->localTransform.position && entity->localTransform.scale.x != 0)
                        {
                            hideCoins(entity);
                            // increase the player score by 10
                            player->score = player->score + 10;
                            playerScore = player->score;
                            this->lastCoinPosition = entity->localTransform.position;
                        }
                        // std::cout << "my score =" << player->score << std::endl;
                        // std::cout << "collided with " << entity->name << " " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                    }
                    if (entity->getComponent<KeyComponent>())
                    {
                        // if the object is an obstacle
                        if (this->lastKeyPosition != entity->localTransform.position && entity->localTransform.scale.x != 0)
                        {
                            hideCoins(entity);
                            this->lastKeyPosition = entity->localTransform.position;
                            // if you have 3 hearts take +100 score else increase hearts
                            if (player->hearts == 3)
                            {
                                player->score = player->score + 100;
                                playerScore = player->score;
                            }
                            else
                            {
                                player->hearts = player->hearts + 1;
                                for (auto heartEntity : world->getEntities())
                                {
                                    HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
                                    if (heart && heart->heartNumber == lastRemovedheart)
                                    {
                                        heartEntity->localTransform.scale.x = 0.0f;
                                        heartEntity->localTransform.scale.y = 0.01f;
                                        heartEntity->localTransform.scale.z = 0.009f;
                                        break;
                                    }
                                }
                            }
                        }
                        // std::cout << "my score =" << player->score << std::endl;
                        // std::cout << "collided with " << entity->name << " " << objectPosition.x << " " << objectPosition.y << " " << objectPosition.z << std::endl;
                    }
                    if (entity->getComponent<EndLineComponent>())
                    { // if the object is an obstacle

                        // std::cout << "congratulation you won with score" << player->score << std::endl;
                        app->changeState("win");
                    }
                    return false;
                }
            }
        }
        return false;
    }

    void CollisionSystem::decreaseHearts(World *world, int count)
    {
        for (auto heartEntity : world->getEntities())
        {
            HeartComponent *heart = heartEntity->getComponent<HeartComponent>();
            if (heart && heart->heartNumber == count)
            {
                heartEntity->localTransform.scale.x = 0.0f;
                heartEntity->localTransform.scale.y = 0.0f;
                heartEntity->localTransform.scale.z = 0.0f;
                break;
            }
        }
    }
    void CollisionSystem::hideCoins(Entity *entity)
    {

        entity->localTransform.scale.x = 0.0f;
        entity->localTransform.scale.y = 0.0f;
        entity->localTransform.scale.z = 0.0f;
    }
}