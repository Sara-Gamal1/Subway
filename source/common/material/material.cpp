#include "material.hpp"
#include "../asset-loader.hpp"
#include "deserialize-utils.hpp"

namespace our
{

    // This function should setup the pipeline state and set the shader to be used
    void Material::setup() const
    {
        // TODO: (Req 7) Write this function
        pipelineState.setup();
        shader->use();
    }

    // This function read the material data from a json object
    void Material::deserialize(const nlohmann::json &data)
    {
        if (!data.is_object())
            return;

        if (data.contains("pipelineState"))
        {
            pipelineState.deserialize(data["pipelineState"]);
        }
        shader = AssetLoader<ShaderProgram>::get(data["shader"].get<std::string>());
        transparent = data.value("transparent", false);
    }

    // This function should call the setup of its parent and
    // set the "tint" uniform to the value in the member variable tint
    void TintedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        Material::setup();
        shader->set("tint", tint);
    }

    // This function read the material data from a json object
    void TintedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        tint = data.value("tint", glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    }

    // This function should call the setup of its parent and
    // set the "alphaThreshold" uniform to the value in the member variable alphaThreshold
    // Then it should bind the texture and sampler to a texture unit and send the unit number to the uniform variable "tex"
    void TexturedMaterial::setup() const
    {
        // TODO: (Req 7) Write this function
        TintedMaterial::setup();
        shader->set("alphaThreshold", alphaThreshold);
        glActiveTexture(GL_TEXTURE0);
        texture->bind();

        if (sampler)
            sampler->bind(0);

        shader->set("tex", 0);
    }

    // This function read the material data from a json object
    void TexturedMaterial::deserialize(const nlohmann::json &data)
    {
        TintedMaterial::deserialize(data);
        if (!data.is_object())
            return;
        alphaThreshold = data.value("alphaThreshold", 0.0f);
        texture = AssetLoader<Texture2D>::get(data.value("texture", ""));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", ""));
    }

    void LightedMaterial::setup() const
    {
        Material::setup();
        glActiveTexture(GL_TEXTURE0);
        albedo->bind();
        sampler->bind(0);
        shader->set("material.albedo", 0);

        glActiveTexture(GL_TEXTURE1);
        ambient_occlusion->bind();
        sampler->bind(1);
        shader->set("material.ambient_occlusion", 1);

        glActiveTexture(GL_TEXTURE2);
        roughness->bind();
        sampler->bind(2);
        shader->set("material.roughness", 2);

        glActiveTexture(GL_TEXTURE3);
        emissive->bind();
        sampler->bind(3);
        shader->set("material.emissive", 3);

        glActiveTexture(GL_TEXTURE4);
        specular->bind();
        sampler->bind(4);
        shader->set("material.specular", 4);
    }

    // This function read the material data from a json object
    void LightedMaterial::deserialize(const nlohmann::json &data)
    {
        Material::deserialize(data);
        if (!data.is_object())
            return;
        albedo = AssetLoader<Texture2D>::get(data.value("albedo", "black"));
        ambient_occlusion = AssetLoader<Texture2D>::get(data.value("ambient_occlusion", "black"));
        roughness = AssetLoader<Texture2D>::get(data.value("roughness", "black"));
        emissive = AssetLoader<Texture2D>::get(data.value("emissive", "black"));
        specular = AssetLoader<Texture2D>::get(data.value("specular", "black"));
        sampler = AssetLoader<Sampler>::get(data.value("sampler", "default"));
    }

}