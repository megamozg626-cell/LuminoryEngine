#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include "../Animation/Skeleton.hpp"
#include "../Renderer/Mesh.hpp"
#include "ModelImporter.hpp"
#include <memory>
#include <map>
#include <string>

namespace Luminary {

class SkeletonManager {
public:
    SkeletonManager();
    ~SkeletonManager();

    // Create skeleton from imported data
    std::shared_ptr<Skeleton> CreateSkeletonFromData(const ModelImporter::SkeletonData& data);
    
    // Get or create built-in skeletons
    std::shared_ptr<Skeleton> GetHumanoidSkeleton();
    std::shared_ptr<Skeleton> GetQuadrupedSkeleton();
    std::shared_ptr<Skeleton> GetBipedSkeleton();
    
    // Save and load skeleton to/from file
    bool SaveSkeleton(const std::string& filepath, const std::shared_ptr<Skeleton>& skeleton);
    std::shared_ptr<Skeleton> LoadSkeleton(const std::string& filepath);
    
    // Skeleton utilities
    std::shared_ptr<Skeleton> CloneSkeleton(const std::shared_ptr<Skeleton>& skeleton);
    bool ValidateSkeleton(const std::shared_ptr<Skeleton>& skeleton) const;

private:
    std::map<std::string, std::shared_ptr<Skeleton>> m_SkeletonCache;
};

} // namespace Luminary
