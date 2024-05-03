// TODO: (phase 2) add game over state

#pragma once

#include "../common/application.hpp"
#include "../common/shader/shader.hpp"
#include "../common/texture/texture2d.hpp"
#include "../common/texture/texture-utils.hpp"
#include "../common/material/material.hpp"
#include "../common/mesh/mesh.hpp"
#include "./menu-state.hpp"


#include <functional>
#include <array>

// This state shows how to use some of the abstractions we created to make a menu.
class GameOverstate : public our::State
{
    // This material will be used to specify shaders , ..  for menu
    our::TexturedMaterial *MenuMaterial;
    // This material will be used to specify shaders , .. for highlighted buttons for menu
    our::TintedMaterial *HighlightedMaterial;
    // rectangle that will be used to draw our material
    our::Mesh *rectangle;
    // Array of button that will interact with during state
    std::array<Button, 1> buttons;

    float time;

    void onInitialize() override
    {
        // Create material for menu
        MenuMaterial = new our::TexturedMaterial();
        // create shaders for our background menu
        MenuMaterial->shader = new our::ShaderProgram();
        MenuMaterial->shader->attach("assets/shaders/textured.vert", GL_VERTEX_SHADER);
        MenuMaterial->shader->attach("assets/shaders/textured.frag", GL_FRAGMENT_SHADER);
        MenuMaterial->shader->link();
        MenuMaterial->texture = our::texture_utils::loadImage("assets/textures/gameover.jpg");
        MenuMaterial->tint = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        HighlightedMaterial = new our::TintedMaterial();
        HighlightedMaterial->shader = new our::ShaderProgram();
        HighlightedMaterial->shader->attach("assets/shaders/tinted.vert", GL_VERTEX_SHADER);
        HighlightedMaterial->shader->attach("assets/shaders/tinted.frag", GL_FRAGMENT_SHADER);
        HighlightedMaterial->shader->link();
        HighlightedMaterial->tint = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        HighlightedMaterial->pipelineState.blending.enabled = true;
        HighlightedMaterial->pipelineState.blending.equation = GL_FUNC_SUBTRACT;
        HighlightedMaterial->pipelineState.blending.sourceFactor = GL_ONE;
        HighlightedMaterial->pipelineState.blending.destinationFactor = GL_ONE;

        rectangle = new our::Mesh({
            {{0.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{1.0f, 0.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}},
            {{1.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},
            {{0.0f, 1.0f, 0.0f}, {255, 255, 255, 255}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}},},
            {0,1,2,2,3,0,});
        time = 0;
        buttons[0].position = {380.0f, 100.0f};
        buttons[0].size = {480.0f, 65.0f};
        buttons[0].action = [this](){ this->getApp()->changeState("play"); };
    }
    void onDraw(double deltaTime) override{
        // Get a reference to the keyboard object
        auto &keyboard = getApp()->getKeyboard();

        if (keyboard.justPressed(GLFW_KEY_SPACE))
        {
            // If the space key is pressed in this frame, go to the play state
            getApp()->changeState("play");
        }
        else if (keyboard.justPressed(GLFW_KEY_ESCAPE))
        {
            // If the escape key is pressed in this frame, exit the game
            getApp()->changeState("menu");
        }

        // Get a reference to the mouse object and get the current mouse position
        auto &mouse = getApp()->getMouse();
        glm::vec2 mousePosition = mouse.getMousePosition();

        // If the mouse left-button is just pressed, check if the mouse was inside
        // any menu button. If it was inside a menu button, run the action of the button.
        if (mouse.justPressed(0))
        {
            for (auto &button : buttons)
            {
                if (button.isInside(mousePosition))
                    button.action();
            }
        }
        // Get the framebuffer size to set the viewport and the create the projection matrix.
        glm::ivec2 size = getApp()->getFrameBufferSize();
        // Make sure the viewport covers the whole size of the framebuffer.
        glViewport(0, 0, size.x, size.y);

        // The view matrix is an identity (there is no camera that moves around).
        // The projection matrix applys an orthographic projection whose size is the framebuffer size in pixels
        // so that the we can define our object locations and sizes in pixels.
        // Note that the top is at 0.0 and the bottom is at the framebuffer height. This allows us to consider the top-left
        // corner of the window to be the origin which makes dealing with the mouse input easier.
        glm::mat4 VP = glm::ortho(0.0f, (float)size.x, (float)size.y, 0.0f, 1.0f, -1.0f);
        // The local to world (model) matrix of the background which is just a scaling matrix to make the menu cover the whole
        // window. Note that we defind the scale in pixels.
        glm::mat4 M = glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        // First, we apply the fading effect.
        time += (float)deltaTime;
        MenuMaterial->tint = glm::vec4(glm::smoothstep(0.00f, 2.00f, time));
        // Then we render the menu background
        // Notice that I don't clear the screen first, since I assume that the menu rectangle will draw over the whole
        // window anyway.
        MenuMaterial->setup();
        MenuMaterial->shader->set("transform", VP * M);
        rectangle->draw();

        // For every button, check if the mouse is inside it. If the mouse is inside, we draw the highlight rectangle over it.
        for (auto &button : buttons)
        {
            if (button.isInside(mousePosition))
            {
                HighlightedMaterial->setup();
                HighlightedMaterial->shader->set("transform", VP * button.getLocalToWorld());
                rectangle->draw();
            }
        }
    }
    void onDestroy() override{
        delete rectangle;
        delete MenuMaterial->texture;
        delete MenuMaterial->shader;
        delete MenuMaterial;
        delete HighlightedMaterial->shader;
        delete HighlightedMaterial;
    }
};