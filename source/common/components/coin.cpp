#include "coin.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "component-deserializer.hpp"

namespace our
{
    
    void CoinComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        // this->speed = data.value("speed", this->speed);
        if (data.contains("components"))
        {
            if (const auto &components = data["components"]; components.is_array())
            {
                for (auto &component : components)
                {
                    deserializeComponent(component, (Entity *)this);
                }
            }
        }
    }
}