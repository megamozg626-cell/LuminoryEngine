#include "ModelImporter.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include <fstream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;

namespace Luminary {

ModelImporter::ModelImporter() = default;
ModelImporter::~ModelImporter() = default;

ModelImporter::ModelFormat ModelImporter::DetectFormat(const std::string& filepath) {
    std::string ext = filepath.substr(filepath.find_last_of(".") + 1);
    
    // Convert to lowercase
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    
    if (ext == "fbx") return ModelFormat::FBX;
    if (ext == "gltf") return ModelFormat::GLTF;
    if (ext == "glb") return ModelFormat::GLB;
    if (ext == "obj") return ModelFormat::OBJ;
    if (ext == "dae") return ModelFormat::DAE;
    if (ext == "blend") return ModelFormat::Blend;
    
    return ModelFormat::Unknown;
}

std::string ModelImporter::GetFormatString(ModelFormat format) {
    switch (format) {
        case ModelFormat::FBX: return "FBX";
        case ModelFormat::GLTF: return "glTF";
        case ModelFormat::GLB: return "glTF Binary";
        case ModelFormat::OBJ: return "Wavefront OBJ";
        case ModelFormat::DAE: return "COLLADA";
        case ModelFormat::Blend: return "Blender";
        default: return "Unknown";
    }
}

bool ModelImporter::LoadModel(const std::string& filepath, ModelData& outModel) {
    if (!fs::exists(filepath)) {
        SetError("File not found: " + filepath);
        LOG_ERROR("Model import failed: {}", GetLastError());
        return false;
    }

    ModelFormat format = DetectFormat(filepath);
    LOG_INFO("Loading model: {} (Format: {})", filepath, GetFormatString(format));

    outModel.SourceFile = filepath;
    outModel.Format = format;

    bool success = false;
    switch (format) {
        case ModelFormat::FBX:
            success = LoadFBX(filepath, outModel);
            break;
        case ModelFormat::GLTF:
            success = LoadGLTF(filepath, outModel);
            break;
        case ModelFormat::GLB:
            success = LoadGLB(filepath, outModel);
            break;
        case ModelFormat::OBJ:
            success = LoadOBJ(filepath, outModel);
            break;
        case ModelFormat::DAE:
            success = LoadDAE(filepath, outModel);
            break;
        default:
            SetError("Unsupported model format: " + filepath);
            success = false;
    }

    if (success && ValidateModel(outModel)) {
        LOG_INFO("Model loaded successfully: {} meshes, {} bones, {} animations",
            outModel.Meshes.size(),
            outModel.Skeleton.Bones.size(),
            outModel.Animations.size());
    } else {
        SetError("Model validation failed for: " + filepath);
        success = false;
    }

    return success;
}

bool ModelImporter::LoadFBX(const std::string& filepath, ModelData& outModel) {
    LOG_DEBUG("Loading FBX model: {}", filepath);
    
    // TODO: Implement using FBX SDK
    // For now, create a placeholder implementation
    outModel.Name = "FBX_Model";
    
    // Create a default cube mesh as placeholder
    MeshData meshData;
    meshData.Name = "Mesh_0";
    meshData.Vertices = {
        -1, -1,  1,  // 0
         1, -1,  1,  // 1
         1,  1,  1,  // 2
        -1,  1,  1,  // 3
        -1, -1, -1,  // 4
         1, -1, -1,  // 5
         1,  1, -1,  // 6
        -1,  1, -1   // 7
    };
    
    meshData.Indices = {
        0, 1, 2, 2, 3, 0,  // front
        4, 6, 5, 4, 7, 6,  // back
        4, 0, 3, 3, 7, 4,  // left
        1, 5, 6, 6, 2, 1,  // right
        3, 2, 6, 6, 7, 3,  // top
        4, 5, 1, 1, 0, 4   // bottom
    };
    
    outModel.Meshes.push_back(meshData);
    
    // Create a default humanoid skeleton
    outModel.Skeleton.Name = "Armature";
    outModel.Skeleton.Bones = {
        {"Root", -1, Vector3(0, 0, 0), Quaternion::Identity(), Vector3::One()},
        {"Pelvis", 0, Vector3(0, 0, 0), Quaternion::Identity(), Vector3::One()},
        {"Spine", 1, Vector3(0, 1, 0), Quaternion::Identity(), Vector3::One()},
        {"Chest", 2, Vector3(0, 1.5, 0), Quaternion::Identity(), Vector3::One()},
        {"Head", 3, Vector3(0, 2, 0), Quaternion::Identity(), Vector3::One()},
    };
    
    LOG_INFO("FBX model loaded (placeholder): {} meshes", outModel.Meshes.size());
    return true;
}

bool ModelImporter::LoadGLTF(const std::string& filepath, ModelData& outModel) {
    LOG_DEBUG("Loading glTF model: {}", filepath);
    
    // TODO: Implement using tinygltf or similar library
    outModel.Name = "glTF_Model";
    
    // Placeholder implementation
    MeshData meshData;
    meshData.Name = "glTF_Mesh";
    outModel.Meshes.push_back(meshData);
    
    LOG_INFO("glTF model loaded (placeholder)");
    return true;
}

bool ModelImporter::LoadGLB(const std::string& filepath, ModelData& outModel) {
    LOG_DEBUG("Loading glTF Binary model: {}", filepath);
    
    // GLB is just binary glTF
    return LoadGLTF(filepath, outModel);
}

bool ModelImporter::LoadOBJ(const std::string& filepath, ModelData& outModel) {
    LOG_DEBUG("Loading OBJ model: {}", filepath);
    
    std::ifstream file(filepath);
    if (!file.is_open()) {
        SetError("Failed to open OBJ file: " + filepath);
        return false;
    }

    MeshData meshData;
    meshData.Name = "OBJ_Mesh";
    
    std::vector<Vector3> positions;
    std::vector<Vector3> normals;
    std::vector<Vector3> texCoords;
    
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;
        
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back(Vector3(x, y, z));
        }
        else if (prefix == "vn") {
            float x, y, z;
            iss >> x >> y >> z;
            normals.push_back(Vector3(x, y, z));
        }
        else if (prefix == "vt") {
            float u, v;
            iss >> u >> v;
            texCoords.push_back(Vector3(u, v, 0));
        }
        else if (prefix == "f") {
            // Parse face indices
            std::string vertex;
            while (iss >> vertex) {
                std::replace(vertex.begin(), vertex.end(), '/', ' ');
                std::istringstream vertexStream(vertex);
                uint32_t posIdx, texIdx = 0, normIdx = 0;
                vertexStream >> posIdx >> texIdx >> normIdx;
                
                if (posIdx > 0 && posIdx <= positions.size()) {
                    meshData.Indices.push_back(posIdx - 1);
                }
            }
        }
    }
    
    file.close();
    
    // Convert positions to float array
    for (const auto& pos : positions) {
        meshData.Vertices.push_back(pos.x);
        meshData.Vertices.push_back(pos.y);
        meshData.Vertices.push_back(pos.z);
    }
    
    // Convert normals to float array
    for (const auto& norm : normals) {
        meshData.Normals.push_back(norm.x);
        meshData.Normals.push_back(norm.y);
        meshData.Normals.push_back(norm.z);
    }
    
    outModel.Meshes.push_back(meshData);
    outModel.Name = "OBJ_Model";
    
    LOG_INFO("OBJ model loaded: {} vertices, {} indices",
        positions.size(), meshData.Indices.size());
    
    return true;
}

bool ModelImporter::LoadDAE(const std::string& filepath, ModelData& outModel) {
    LOG_DEBUG("Loading COLLADA model: {}", filepath);
    
    // TODO: Implement using COLLADA parser
    outModel.Name = "DAE_Model";
    
    // Placeholder implementation
    MeshData meshData;
    meshData.Name = "DAE_Mesh";
    outModel.Meshes.push_back(meshData);
    
    LOG_INFO("COLLADA model loaded (placeholder)");
    return true;
}

bool ModelImporter::ImportMeshes(const std::string& filepath, std::vector<MeshData>& outMeshes) {
    ModelData modelData;
    if (LoadModel(filepath, modelData)) {
        outMeshes = modelData.Meshes;
        return true;
    }
    return false;
}

bool ModelImporter::ImportSkeleton(const std::string& filepath, SkeletonData& outSkeleton) {
    ModelData modelData;
    if (LoadModel(filepath, modelData)) {
        outSkeleton = modelData.Skeleton;
        return true;
    }
    return false;
}

bool ModelImporter::ImportAnimations(const std::string& filepath, std::vector<AnimationData>& outAnimations) {
    ModelData modelData;
    if (LoadModel(filepath, modelData)) {
        outAnimations = modelData.Animations;
        return true;
    }
    return false;
}

bool ModelImporter::ValidateModel(const ModelData& model) const {
    if (model.Meshes.empty()) {
        LOG_WARNING("Model has no meshes");
        return false;
    }
    
    for (const auto& mesh : model.Meshes) {
        if (mesh.Vertices.empty()) {
            LOG_WARNING("Mesh '{}' has no vertices", mesh.Name);
            return false;
        }
        if (mesh.Indices.empty()) {
            LOG_WARNING("Mesh '{}' has no indices", mesh.Name);
            return false;
        }
    }
    
    return true;
}

} // namespace Luminary
