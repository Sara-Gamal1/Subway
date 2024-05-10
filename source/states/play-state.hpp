#pragma once

#include <application.hpp>

#include <ecs/world.hpp>
#include <systems/forward-renderer.hpp>
#include <systems/free-camera-controller.hpp>
#include <systems/movement.hpp>
#include <asset-loader.hpp>
#include <systems/collision.hpp>
#include <iostream>

// This state shows how to use the ECS framework and deserialization.
class Playstate : public our::State
{
    public:
    our::World world;
    our::ForwardRenderer renderer;
    our::FreeCameraControllerSystem cameraController;
    our::MovementSystem movementSystem;
    our::CollisionSystem collisionSystem;
    int playerScore = 0;
    int level;

    void onInitialize() override
    {
        // First of all, we get the scene configuration from the app config
        world.level = this->level;
        auto &config = getApp()->getConfig()["scene"];
        // If we have assets in the scene config, we deserialize them
        if (config.contains("assets"))
        {
            our::deserializeAllAssets(config["assets"]);
        }
        // If we have a world in the scene config, we use it to populate our world
        if (config.contains("world"))
        {
            world.deserialize(config["world"]);
        }
        // We initialize the camera controller system since it needs a pointer to the app
        cameraController.enter(getApp());
        collisionSystem.enter(getApp());
        // Then we initialize the renderer
        auto size = getApp()->getFrameBufferSize();
        renderer.initialize(size, config["renderer"]);
    }

    void onDraw(double deltaTime) override
    {
        bool didCollide = false;
        // Here, we just run a bunch of systems to control the world logic
        movementSystem.update(&world, (float)deltaTime);
        cameraController.update(&world, (float)deltaTime);
        didCollide = collisionSystem.update(&world, 0, 0, 0, playerScore);
        // And finally we use the renderer system to draw the scene
        renderer.render(&world);

        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape  key is pressed in this frame, go to the play state
            getApp()->changeState("menu");
        }
    }

    void onDestroy() override
    {
        // Don't forget to destroy the renderer
        renderer.destroy();
        // On exit, we call exit for the camera controller system to make sure that the mouse is unlocked
        cameraController.exit();
        // Clear the world
        world.clear();
        // and we delete all the loaded assets to free memory on the RAM and the VRAM
        our::clearAllAssets();
    }
    void onImmediateGui()
    {
        // to show U more menus (useful for debugging)
        // renderer.showGUI(&world);
        // start gui
        ImGui::Begin("Score", 0, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration);
        // set window position
        ImGui::SetWindowPos(ImVec2(10, 10));
        // set window size
        ImGui::SetWindowSize(ImVec2(600, 100));
        // set font
        ImGui::SetWindowFontScale(5.0f);
        // initialize score
        std::string score_screen = "Score: " + std::to_string(playerScore);
        // initialize color
        ImGui::TextColored(ImVec4(0.957f, 0.352f, 0.0f, 1.0f), score_screen.c_str());
        // end gui
        ImGui::End();
    }
};