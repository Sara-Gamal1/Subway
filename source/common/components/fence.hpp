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
    class FenceComponent : public Component {
    public:
        Application *app;
        static std::string getID() { return "fence"; }

        void enter(Application *app)
        {
            this->app = app;
        }

        void deserialize(const nlohmann::json& data) override;
    };

}