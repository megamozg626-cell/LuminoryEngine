#pragma once

#include <memory>
#include <vector>
#include <string>

namespace Luminary {

class Material;

class Mesh {
public:
    struct Vertex {
        float position[3];
        float normal[3];
        float texCoord[2];
    };

    Mesh();
    ~Mesh();

    void SetVertices(const std::vector<Vertex>& vertices) { m_Vertices = vertices; }
    void SetIndices(const std::vector<uint32_t>& indices) { m_Indices = indices; }
    void SetMaterial(std::shared_ptr<Material> material) { m_Material = material; }

    const std::vector<Vertex>& GetVertices() const { return m_Vertices; }
    const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
    std::shared_ptr<Material> GetMaterial() const { return m_Material; }

    uint32_t GetVertexCount() const { return static_cast<uint32_t>(m_Vertices.size()); }
    uint32_t GetIndexCount() const { return static_cast<uint32_t>(m_Indices.size()); }

    static std::shared_ptr<Mesh> CreateCube();
    static std::shared_ptr<Mesh> CreateSphere(uint32_t segments = 32, uint32_t rings = 16);
    static std::shared_ptr<Mesh> CreatePlane(float width = 10.0f, float height = 10.0f);

private:
    std::vector<Vertex> m_Vertices;
    std::vector<uint32_t> m_Indices;
    std::shared_ptr<Material> m_Material;
};

} // namespace Luminary
