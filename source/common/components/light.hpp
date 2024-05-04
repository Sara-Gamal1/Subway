#pragma once
#include "../ecs/component.hpp"
#include <glm/glm.hpp>
namespace our
{
    class LightComponent : public Component
    {
    public:
        int light_type; 
        // 0->directional
        // 1->point
        // 2->spot
        glm::vec3 color;
        glm::vec3 attenuation;
        glm::vec2 cone_angles;
        static std::string getID() { return "LightComponent"; }
        void deserialize(const nlohmann::json &data) override;
    };

}