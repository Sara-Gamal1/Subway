#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/dog.hpp"
#include "../components/free-camera-controller.hpp"
#include "../components/player.hpp"
#include "../components/police.hpp"
#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>


namespace our
{
    enum JumpState
    {
        JUMPING,
        FALLING,
        GROUNDED
    };
    // The free camera controller system is responsible for moving every entity which contains a FreeCameraControllerComponent.
    // This system is added as a slightly complex example for how use the ECS framework to implement logic.
    // For more information, see "common/components/free-camera-controller.hpp"
    class FreeCameraControllerSystem
    {
        Application *app;          // The application in which the state runs
        bool mouse_locked = false; // Is the mouse locked
        bool key1_pressed = false;
        bool key2_pressed = false;
        our::JumpState jumpState = our::JumpState::GROUNDED;
        int collisionFactor = 2;

        float jumpSpeed = 12;
        float jumpMaxHeight = 10;

    public:
        // When a state enters, it should call this function and give it the pointer to the application
        void enter(Application *app)
        {
            this->app = app;
        }

        // This should be called every frame to update all entities containing a FreeCameraControllerComponent
        void update(World *world, float deltaTime)
        {

            Entity *playerEntity;    // The player entity if it exists
            PlayerComponent *player; // The player component if it exists
            Entity *dogEntity;       // The dog entity if it exists
            Entity *policeEntity;    // The dog entity if it exists
            DogComponent *dog;       // The dog component if it exists
            PoliceComponent *police;    

            for (auto entity : world->getEntities())
            {
                // search for the dog entity
                // Get the dog component if it exists
                dogEntity = entity;
                dog = dogEntity->getComponent<DogComponent>();
                if (dog)
                    break;
                // If the dog component exists
            }

            for (auto entity : world->getEntities())
            {
                // search for the dog entity
                // Get the dog component if it exists
                policeEntity = entity;
                police = policeEntity->getComponent<PoliceComponent>();
                if (police)
                    break;
                // If the dog component exists
            }

            for (auto entity : world->getEntities())
            {
                // search for the player entity
                // Get the player component if it exists
                playerEntity = entity;
                player = playerEntity->getComponent<PlayerComponent>();
                if (player )
                    break;
                // If the player component exists
            }


            // First of all, we search for an entity containing both a CameraComponent and a FreeCameraControllerComponent
            // As soon as we find one, we break
            CameraComponent *camera = nullptr;
            FreeCameraControllerComponent *controller = nullptr;
            for (auto entity : world->getEntities())
            {
                camera = entity->getComponent<CameraComponent>();
                controller = entity->getComponent<FreeCameraControllerComponent>();
                if (camera && controller)
                    break;
            }
            // If there is no entity with both a CameraComponent and a FreeCameraControllerComponent, we can do nothing so we return
            if (!(camera && controller))
                return;
            // Get the entity that we found via getOwner of camera (we could use controller->getOwner())
            Entity *entity = camera->getOwner();

            // If the left mouse button is pressed, we lock and hide the mouse. This common in First Person Games.
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && !mouse_locked)
            {
                app->getMouse().lockMouse(app->getWindow());
                mouse_locked = true;
                // If the left mouse button is released, we unlock and unhide the mouse.
            }
            else if (!app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1) && mouse_locked)
            {
                app->getMouse().unlockMouse(app->getWindow());
                mouse_locked = false;
            }

            // We get a reference to the entity's position and rotation
            glm::vec3 &position = entity->localTransform.position;
            glm::vec3 &positionjake = playerEntity->localTransform.position;
            glm::vec3 &positionDog = dogEntity->localTransform.position;
            glm::vec3 &positionPolice = policeEntity->localTransform.position;
            glm::vec3 &rotation = entity->localTransform.rotation;

            // If the left mouse button is pressed, we get the change in the mouse location
            // and use it to update the camera rotation
            if (app->getMouse().isPressed(GLFW_MOUSE_BUTTON_1))
            {
                glm::vec2 delta = app->getMouse().getMouseDelta();
                rotation.x -= delta.y * controller->rotationSensitivity; // The y-axis controls the pitch
                rotation.y -= delta.x * controller->rotationSensitivity; // The x-axis controls the yaw
            }

            // We prevent the pitch from exceeding a certain angle from the XZ plane to prevent gimbal locks
            if (rotation.x < -glm::half_pi<float>() * 0.99f)
                rotation.x = -glm::half_pi<float>() * 0.99f;
            if (rotation.x > glm::half_pi<float>() * 0.99f)
                rotation.x = glm::half_pi<float>() * 0.99f;
            // This is not necessary, but whenever the rotation goes outside the 0 to 2*PI range, we wrap it back inside.
            // This could prevent floating point error if the player rotates in single direction for an extremely long time.
            rotation.y = glm::wrapAngle(rotation.y);

            // We update the camera fov based on the mouse wheel scrolling amount
            float fov = camera->fovY + app->getMouse().getScrollOffset().y * controller->fovSensitivity;
            fov = glm::clamp(fov, glm::pi<float>() * 0.01f, glm::pi<float>() * 0.99f); // We keep the fov in the range 0.01*PI to 0.99*PI
            camera->fovY = fov;

            // We get the camera model matrix (relative to its parent) to compute the front, up and right directions
            glm::mat4 matrix = entity->localTransform.toMat4();

            glm::vec3 front = glm::vec3(matrix * glm::vec4(0, 0, -1, 0)),
                      up = glm::vec3(matrix * glm::vec4(0, 1, 0, 0)),
                      right = glm::vec3(matrix * glm::vec4(1, 0, 0, 0));

            glm::vec3 current_sensitivity = controller->positionSensitivity;
            // If the LEFT SHIFT key is pressed, we multiply the position sensitivity by the speed up factor
            if (app->getKeyboard().isPressed(GLFW_KEY_LEFT_SHIFT))
                current_sensitivity *= controller->speedupFactor;

            if (app->getKeyboard().isPressed(GLFW_KEY_SPACE))
            {
                if (jumpState == our::JumpState::GROUNDED)
                {
                    // We set the jump state to JUMPING
                    jumpState = our::JumpState::JUMPING;
                    // Start the jump
                    positionjake.y += (deltaTime * jumpSpeed * collisionFactor);
                    positionjake.z -= (deltaTime * 4);
                    positionDog.y += (deltaTime * jumpSpeed * collisionFactor);
                    positionDog.z -= (deltaTime * 4);
                    positionPolice.y += (deltaTime * jumpSpeed * collisionFactor);
                    positionPolice.z -= (deltaTime * 4);

                    position.z -= (deltaTime * 4);
                }
            }
            // If the player jumps higher than the max height, we set the jump state to FALLING
            if (positionjake.y >= jumpMaxHeight)
            {
                jumpState = our::JumpState::FALLING;
            }
            else if (positionjake.y <= -4)
            {
                // If the player was falling, we set the jump state to GROUNDED
                jumpState = our::JumpState::GROUNDED;
            }

            // We update the player position based on the jump state
            if (jumpState == our::JumpState::JUMPING)
            {
                positionjake.y += (deltaTime * jumpSpeed);
                positionjake.z -= deltaTime * 4;
                positionDog.y += (deltaTime * jumpSpeed);
                positionDog.z -= deltaTime * 4;

                positionPolice.y += (deltaTime * jumpSpeed);
                positionPolice.z -= deltaTime * 4;
                position.z -= deltaTime * 4;
            }
            else if (jumpState == our::JumpState::FALLING)
            {
                // We update the player position based on the jump state
                positionjake.y -= (deltaTime * jumpSpeed);
                positionjake.z -= deltaTime * 4;
                positionDog.y -= (deltaTime * jumpSpeed);
                positionDog.z -= deltaTime * 4;
                positionPolice.y -= (deltaTime * jumpSpeed);
                positionPolice.z -= deltaTime * 4;
                position.z -= deltaTime * 4;
            }
            else
            {
                // We make sure the player is grounded
                positionjake.y = -4;
                positionDog.y = -4;
                positionPolice.y = -4;
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_D) && jumpState == our::JumpState::GROUNDED)
            {
                if (!key1_pressed)
                {
                    key1_pressed = true;
                    if (positionjake.x < 8)
                        {
                            positionjake.x += 8;
                            positionDog.x += 8;
                            positionPolice.x += 8;
                        }
                }
            }
            else
            {
                if (key1_pressed)
                {
                    key1_pressed = false;
                }
            }
            if (app->getKeyboard().isPressed(GLFW_KEY_A) && jumpState == our::JumpState::GROUNDED)
            {
                if (!key2_pressed)
                {
                    key2_pressed = true;
                    if (positionjake.x > -8)
                        {
                            positionjake.x -= 8;
                            positionDog.x -= 8;
                            positionPolice.x -= 8;
                        }
                }
            }
            else
            {
                if (key2_pressed)
                {
                    key2_pressed = false;
                }
            }
        }
        // When the state exits, it should call this function to ensure the mouse is unlocked
        void exit()
        {
            if (mouse_locked)
            {
                mouse_locked = false;
                app->getMouse().unlockMouse(app->getWindow());
            }
        }
    };

}
