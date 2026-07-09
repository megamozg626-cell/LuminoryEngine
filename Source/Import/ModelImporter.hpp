#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace Luminary {

class Vector3;
class Quaternion;

class ModelImporter {
public:
    enum class ModelFormat {
        FBX,
        GLTF,
        GLB,
        OBJ,
        DAE,
        Blend,
        Unknown
    };

    struct MeshData {
        std::string Name;
        std::vector<float> Vertices;      // x, y, z for each vertex
        std::vector<float> Normals;       // x, y, z for each normal
        std::vector<float> TexCoords;     // u, v for each vertex
        std::vector<uint32_t> Indices;
        std::vector<int32_t> BoneIndices; // bone influence indices
        std::vector<float> BoneWeights;   // bone influence weights
    };

    struct BoneData {
        std::string Name;
        int32_t ParentIndex = -1;
        Vector3 Position;
        Quaternion Rotation;
        Vector3 Scale;
    };

    struct SkeletonData {
        std::string Name;
        std::vector<BoneData> Bones;
    };

    struct AnimationKeyFrame {
        float Time = 0.0f;
        Vector3 Position;
        Quaternion Rotation;
        Vector3 Scale;
    };

    struct BoneAnimationData {
        uint32_t BoneIndex = 0;
        std::vector<AnimationKeyFrame> KeyFrames;
    };

    struct AnimationData {
        std::string Name;
        float Duration = 0.0f;
        float FrameRate = 30.0f;
        std::vector<BoneAnimationData> BoneAnimations;
    };

    struct ModelData {
        std::string Name;
        std::vector<MeshData> Meshes;
        SkeletonData Skeleton;
        std::vector<AnimationData> Animations;
        std::string SourceFile;
        ModelFormat Format;
    };

    ModelImporter();
    ~ModelImporter();

    // Load model from file
    bool LoadModel(const std::string& filepath, ModelData& outModel);
    
    // Detect format from file extension
    static ModelFormat DetectFormat(const std::string& filepath);
    static std::string GetFormatString(ModelFormat format);

    // Import specific data
    bool ImportMeshes(const std::string& filepath, std::vector<MeshData>& outMeshes);
    bool ImportSkeleton(const std::string& filepath, SkeletonData& outSkeleton);
    bool ImportAnimations(const std::string& filepath, std::vector<AnimationData>& outAnimations);

    // Validation
    bool ValidateModel(const ModelData& model) const;
    std::string GetLastError() const { return m_LastError; }

private:
    std::string m_LastError;
    
    bool LoadFBX(const std::string& filepath, ModelData& outModel);
    bool LoadGLTF(const std::string& filepath, ModelData& outModel);
    bool LoadGLB(const std::string& filepath, ModelData& outModel);
    bool LoadOBJ(const std::string& filepath, ModelData& outModel);
    bool LoadDAE(const std::string& filepath, ModelData& outModel);
    
    void SetError(const std::string& error) { m_LastError = error; }
};

} // namespace Luminary
