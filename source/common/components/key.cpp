#include "key.hpp"
#include "../ecs/entity.hpp"
#include "../deserialize-utils.hpp"
#include "component-deserializer.hpp"
#include<iostream>
namespace our
{
    void KeyComponent::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;
        this->heartNumber = data.value("number", this->heartNumber);
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