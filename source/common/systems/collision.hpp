#pragma once

#include "../ecs/world.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>
#include "../application.hpp"

namespace our
{
    // The collision system is responsible for detecting collisions between entities.
    class CollisionSystem
    {

    public:
        glm::vec3 lastCoinPosition;
        glm::vec3 lastFencePosition;
        CollisionSystem()
        {
        }
        Application *app; // The application in which the state runs
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a MovementComponent.
        bool update(World *world, float deltaTime, int heartCount,
                    float collisionStartTime, int &playerScore);

        // This function is called when the player collides with an obstacle
        void decreaseHearts(World *world, int count);
        void hideCoins(Entity *entity);
    };
}