#include "Mesh.hpp"
#include "../Math/Math.hpp"

namespace Luminary {

Mesh::Mesh() = default;
Mesh::~Mesh() = default;

std::shared_ptr<Mesh> Mesh::CreateCube() {
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices = {
        // Front face
        {{-1, -1,  1}, {0, 0, 1}, {0, 0}},
        {{ 1, -1,  1}, {0, 0, 1}, {1, 0}},
        {{ 1,  1,  1}, {0, 0, 1}, {1, 1}},
        {{-1,  1,  1}, {0, 0, 1}, {0, 1}},
        // Back face
        {{-1, -1, -1}, {0, 0, -1}, {0, 0}},
        {{-1,  1, -1}, {0, 0, -1}, {0, 1}},
        {{ 1,  1, -1}, {0, 0, -1}, {1, 1}},
        {{ 1, -1, -1}, {0, 0, -1}, {1, 0}},
        // Top face
        {{-1,  1, -1}, {0, 1, 0}, {0, 0}},
        {{-1,  1,  1}, {0, 1, 0}, {0, 1}},
        {{ 1,  1,  1}, {0, 1, 0}, {1, 1}},
        {{ 1,  1, -1}, {0, 1, 0}, {1, 0}},
        // Bottom face
        {{-1, -1, -1}, {0, -1, 0}, {0, 0}},
        {{ 1, -1, -1}, {0, -1, 0}, {1, 0}},
        {{ 1, -1,  1}, {0, -1, 0}, {1, 1}},
        {{-1, -1,  1}, {0, -1, 0}, {0, 1}},
        // Right face
        {{ 1, -1, -1}, {1, 0, 0}, {0, 0}},
        {{ 1,  1, -1}, {1, 0, 0}, {0, 1}},
        {{ 1,  1,  1}, {1, 0, 0}, {1, 1}},
        {{ 1, -1,  1}, {1, 0, 0}, {1, 0}},
        // Left face
        {{-1, -1, -1}, {-1, 0, 0}, {0, 0}},
        {{-1, -1,  1}, {-1, 0, 0}, {1, 0}},
        {{-1,  1,  1}, {-1, 0, 0}, {1, 1}},
        {{-1,  1, -1}, {-1, 0, 0}, {0, 1}},
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 0, 2, 3,
        4, 5, 6, 4, 6, 7,
        8, 9, 10, 8, 10, 11,
        12, 13, 14, 12, 14, 15,
        16, 17, 18, 16, 18, 19,
        20, 21, 22, 20, 22, 23
    };

    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreateSphere(uint32_t segments, uint32_t rings) {
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    for (uint32_t ring = 0; ring <= rings; ++ring) {
        float phi = Luminary::Math::PI * ring / rings;
        float sinPhi = std::sin(phi);
        float cosPhi = std::cos(phi);

        for (uint32_t seg = 0; seg <= segments; ++seg) {
            float theta = 2.0f * Luminary::Math::PI * seg / segments;
            float sinTheta = std::sin(theta);
            float cosTheta = std::cos(theta);

            Vertex vertex;
            vertex.position[0] = cosTheta * sinPhi;
            vertex.position[1] = cosPhi;
            vertex.position[2] = sinTheta * sinPhi;
            vertex.normal[0] = vertex.position[0];
            vertex.normal[1] = vertex.position[1];
            vertex.normal[2] = vertex.position[2];
            vertex.texCoord[0] = static_cast<float>(seg) / segments;
            vertex.texCoord[1] = static_cast<float>(ring) / rings;
            vertices.push_back(vertex);
        }
    }

    for (uint32_t ring = 0; ring < rings; ++ring) {
        for (uint32_t seg = 0; seg < segments; ++seg) {
            uint32_t a = ring * (segments + 1) + seg;
            uint32_t b = a + segments + 1;
            indices.push_back(a);
            indices.push_back(b);
            indices.push_back(a + 1);
            indices.push_back(a + 1);
            indices.push_back(b);
            indices.push_back(b + 1);
        }
    }

    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    return mesh;
}

std::shared_ptr<Mesh> Mesh::CreatePlane(float width, float height) {
    auto mesh = std::make_shared<Mesh>();
    std::vector<Vertex> vertices = {
        {{-width / 2, 0, -height / 2}, {0, 1, 0}, {0, 0}},
        {{ width / 2, 0, -height / 2}, {0, 1, 0}, {1, 0}},
        {{ width / 2, 0,  height / 2}, {0, 1, 0}, {1, 1}},
        {{-width / 2, 0,  height / 2}, {0, 1, 0}, {0, 1}},
    };

    std::vector<uint32_t> indices = { 0, 1, 2, 0, 2, 3 };

    mesh->SetVertices(vertices);
    mesh->SetIndices(indices);
    return mesh;
}

} // namespace Luminary
