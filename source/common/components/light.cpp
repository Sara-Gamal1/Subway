#include "./light.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
namespace our
{
    void LightComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        light_type = data.value("light_type", 0);                                       // default value is directional light
        color = data.value("color", glm::vec3(1.0f));                                   // default color is white
        attenuation = data.value("attenuation", glm::vec3(1.0f));                       // default attenuation is 1
        cone_angles = glm::radians(data.value("cone_angles", glm::vec2(30.0f, 45.0f))); // default cone angles is 30  45 deg
    }
}