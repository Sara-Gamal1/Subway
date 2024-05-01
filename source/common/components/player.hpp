#pragma once

#include "../ecs/world.hpp"
#include "../components/camera.hpp"
#include "../components/free-camera-controller.hpp"

#include "../application.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/trigonometric.hpp>
#include <glm/gtx/fast_trigonometry.hpp>

namespace our {
    class PlayerComponent : public Component {
    public:
        glm::float32 speed = 1; // Each frame, the entity should move as follows: position += linearVelocity * deltaTime
        Application *app;
        static std::string getID() { return "player"; }

        void enter(Application *app)
        {
            this->app = app;
        }

        void deserialize(const nlohmann::json& data) override;
    };

}