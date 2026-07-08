#include "Material.hpp"

namespace Luminary {

Shader::Shader(ShaderType type) : m_Type(type) {}

Shader::~Shader() = default;

bool Shader::Compile(const std::string& source) {
    // Shader compilation will be implemented with actual HLSL compilation
    return true;
}

Material::Material() = default;
Material::~Material() = default;

std::shared_ptr<Material> Material::CreateStandard() {
    auto material = std::make_shared<Material>();
    material->SetAlbedo(Vector3(0.8f, 0.8f, 0.8f));
    material->SetMetallic(0.0f);
    material->SetRoughness(0.5f);
    return material;
}

std::shared_ptr<Material> Material::CreateMetallic() {
    auto material = std::make_shared<Material>();
    material->SetAlbedo(Vector3(0.7f, 0.7f, 0.7f));
    material->SetMetallic(1.0f);
    material->SetRoughness(0.2f);
    return material;
}

std::shared_ptr<Material> Material::CreateRough() {
    auto material = std::make_shared<Material>();
    material->SetAlbedo(Vector3(0.6f, 0.6f, 0.6f));
    material->SetMetallic(0.0f);
    material->SetRoughness(0.8f);
    return material;
}

} // namespace Luminary
