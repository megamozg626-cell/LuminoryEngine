#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Vector4.hpp"
#include <string>

namespace Luminary {

class Shader {
public:
    enum class ShaderType {
        Vertex,
        Pixel,
        Compute,
        Geometry,
        Hull,
        Domain
    };

    Shader(ShaderType type);
    ~Shader();

    bool Compile(const std::string& source);
    ShaderType GetType() const { return m_Type; }

private:
    ShaderType m_Type;
};

class Material {
public:
    Material();
    ~Material();

    void SetAlbedo(const Vector3& color) { m_Albedo = color; }
    void SetMetallic(float value) { m_Metallic = value; }
    void SetRoughness(float value) { m_Roughness = value; }
    void SetAmbientOcclusion(float value) { m_AO = value; }

    const Vector3& GetAlbedo() const { return m_Albedo; }
    float GetMetallic() const { return m_Metallic; }
    float GetRoughness() const { return m_Roughness; }
    float GetAmbientOcclusion() const { return m_AO; }

    static std::shared_ptr<Material> CreateStandard();
    static std::shared_ptr<Material> CreateMetallic();
    static std::shared_ptr<Material> CreateRough();

private:
    Vector3 m_Albedo = Vector3(0.8f, 0.8f, 0.8f);
    float m_Metallic = 0.0f;
    float m_Roughness = 0.5f;
    float m_AO = 1.0f;
};

} // namespace Luminary
