#pragma once

#include "../ecs/entity.hpp"
#include "camera.hpp"
#include "mesh-renderer.hpp"
#include "free-camera-controller.hpp"
#include "movement.hpp"
#include "player.hpp"
#include "collision.hpp"
#include "coin.hpp"
#include "train.hpp"
#include "fence.hpp"
#include "end-line.hpp"
#include "heart.hpp"
namespace our
{

    // Given a json object, this function picks and creates a component in the given entity
    // based on the "type" specified in the json object which is later deserialized from the rest of the json object
    inline void deserializeComponent(const nlohmann::json &data, Entity *entity)
    {
        std::string type = data.value("type", "");
        Component *component = nullptr;
        // TODO: (Req 8) Add an option to deserialize a "MeshRendererComponent" to the following if-else statement
        if (type == CameraComponent::getID())
        {
            component = entity->addComponent<CameraComponent>();
        }
        else if (type == FreeCameraControllerComponent::getID())
        {
            component = entity->addComponent<FreeCameraControllerComponent>();
        }
        else if (type == MovementComponent::getID())
        {
            component = entity->addComponent<MovementComponent>();
        }
        else if (type == MeshRendererComponent::getID())
        {
            component = entity->addComponent<MeshRendererComponent>();
        }
        else if (type == PlayerComponent::getID())
        {
            component = entity->addComponent<PlayerComponent>();
        }
        else if (type == CollisionComponent::getID())
        {
            component = entity->addComponent<CollisionComponent>();
        }
        else if (type == CoinComponent::getID())
        {
            component = entity->addComponent<CoinComponent>();
        }
        else if (type == FenceComponent::getID())
        {
            component = entity->addComponent<FenceComponent>();
        }
        else if (type == TrainComponent::getID())
        {
            component = entity->addComponent<TrainComponent>();
        }
        else if (type == EndLineComponent::getID())
        {
            component = entity->addComponent<EndLineComponent>();
        }
        else if (type == HeartComponent::getID())
        {
            component = entity->addComponent<HeartComponent>();
        }
        if (component)
            component->deserialize(data);
    }

}