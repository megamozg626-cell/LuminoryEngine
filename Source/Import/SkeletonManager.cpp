#include "SkeletonManager.hpp"
#include "../Core/Logging/Logger.hpp"
#include <fstream>
#include <sstream>

namespace Luminary {

SkeletonManager::SkeletonManager() {
    LOG_DEBUG("SkeletonManager created");
}

SkeletonManager::~SkeletonManager() = default;

std::shared_ptr<Skeleton> SkeletonManager::CreateSkeletonFromData(const ModelImporter::SkeletonData& data) {
    auto skeleton = std::make_shared<Skeleton>();
    
    LOG_INFO("Creating skeleton from imported data: {}", data.Name);
    
    // Create bones based on imported data
    for (size_t i = 0; i < data.Bones.size(); ++i) {
        const auto& boneData = data.Bones[i];
        uint32_t boneIndex = skeleton->AddBone(boneData.Name, boneData.ParentIndex);
        
        auto& bone = skeleton->GetBone(boneIndex);
        bone.Position = boneData.Position;
        bone.Rotation = boneData.Rotation;
        bone.Scale = boneData.Scale;
        
        LOG_DEBUG("Added bone: {} (Parent: {})", boneData.Name, boneData.ParentIndex);
    }
    
    if (ValidateSkeleton(skeleton)) {
        LOG_INFO("Skeleton created successfully with {} bones", skeleton->GetBoneCount());
        m_SkeletonCache[data.Name] = skeleton;
        return skeleton;
    }
    
    LOG_ERROR("Failed to create skeleton from data");
    return nullptr;
}

std::shared_ptr<Skeleton> SkeletonManager::GetHumanoidSkeleton() {
    const std::string name = "Humanoid";
    
    auto it = m_SkeletonCache.find(name);
    if (it != m_SkeletonCache.end()) {
        return it->second;
    }
    
    auto skeleton = Skeleton::CreateHumanoidSkeleton();
    m_SkeletonCache[name] = skeleton;
    return skeleton;
}

std::shared_ptr<Skeleton> SkeletonManager::GetQuadrupedSkeleton() {
    const std::string name = "Quadruped";
    
    auto it = m_SkeletonCache.find(name);
    if (it != m_SkeletonCache.end()) {
        return it->second;
    }
    
    auto skeleton = std::make_shared<Skeleton>();
    
    // Create quadruped skeleton (4-legged creature)
    uint32_t root = skeleton->AddBone("Root", -1);
    uint32_t spine = skeleton->AddBone("Spine", root);
    uint32_t neck = skeleton->AddBone("Neck", spine);
    uint32_t head = skeleton->AddBone("Head", neck);
    
    // Front legs
    uint32_t frontLeftHip = skeleton->AddBone("FrontLeftHip", spine);
    uint32_t frontLeftLeg = skeleton->AddBone("FrontLeftLeg", frontLeftHip);
    uint32_t frontLeftPaw = skeleton->AddBone("FrontLeftPaw", frontLeftLeg);
    
    uint32_t frontRightHip = skeleton->AddBone("FrontRightHip", spine);
    uint32_t frontRightLeg = skeleton->AddBone("FrontRightLeg", frontRightHip);
    uint32_t frontRightPaw = skeleton->AddBone("FrontRightPaw", frontRightLeg);
    
    // Back legs
    uint32_t backLeftHip = skeleton->AddBone("BackLeftHip", spine);
    uint32_t backLeftLeg = skeleton->AddBone("BackLeftLeg", backLeftHip);
    uint32_t backLeftPaw = skeleton->AddBone("BackLeftPaw", backLeftLeg);
    
    uint32_t backRightHip = skeleton->AddBone("BackRightHip", spine);
    uint32_t backRightLeg = skeleton->AddBone("BackRightLeg", backRightHip);
    uint32_t backRightPaw = skeleton->AddBone("BackRightPaw", backRightLeg);
    
    // Tail
    uint32_t tailBase = skeleton->AddBone("TailBase", spine);
    uint32_t tailMid = skeleton->AddBone("TailMid", tailBase);
    uint32_t tailTip = skeleton->AddBone("TailTip", tailMid);
    
    LOG_INFO("Quadruped skeleton created with {} bones", skeleton->GetBoneCount());
    
    m_SkeletonCache[name] = skeleton;
    return skeleton;
}

std::shared_ptr<Skeleton> SkeletonManager::GetBipedSkeleton() {
    const std::string name = "Biped";
    
    auto it = m_SkeletonCache.find(name);
    if (it != m_SkeletonCache.end()) {
        return it->second;
    }
    
    auto skeleton = std::make_shared<Skeleton>();
    
    // Create biped skeleton (2-legged creature)
    uint32_t root = skeleton->AddBone("Root", -1);
    uint32_t pelvis = skeleton->AddBone("Pelvis", root);
    uint32_t spine = skeleton->AddBone("Spine", pelvis);
    uint32_t chest = skeleton->AddBone("Chest", spine);
    uint32_t neck = skeleton->AddBone("Neck", chest);
    uint32_t head = skeleton->AddBone("Head", neck);
    
    // Left leg
    uint32_t leftHip = skeleton->AddBone("LeftHip", pelvis);
    uint32_t leftKnee = skeleton->AddBone("LeftKnee", leftHip);
    uint32_t leftAnkle = skeleton->AddBone("LeftAnkle", leftKnee);
    
    // Right leg
    uint32_t rightHip = skeleton->AddBone("RightHip", pelvis);
    uint32_t rightKnee = skeleton->AddBone("RightKnee", rightHip);
    uint32_t rightAnkle = skeleton->AddBone("RightAnkle", rightKnee);
    
    LOG_INFO("Biped skeleton created with {} bones", skeleton->GetBoneCount());
    
    m_SkeletonCache[name] = skeleton;
    return skeleton;
}

bool SkeletonManager::SaveSkeleton(const std::string& filepath, const std::shared_ptr<Skeleton>& skeleton) {
    if (!skeleton) {
        LOG_ERROR("Cannot save null skeleton");
        return false;
    }
    
    std::ofstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open file for saving: {}", filepath);
        return false;
    }
    
    // Save skeleton data
    uint32_t boneCount = skeleton->GetBoneCount();
    file << "SKELETON\n";
    file << "BoneCount: " << boneCount << "\n\n";
    
    for (uint32_t i = 0; i < boneCount; ++i) {
        const auto& bone = skeleton->GetBone(i);
        file << "Bone: " << bone.Name << "\n";
        file << "  ParentIndex: " << bone.ParentIndex << "\n";
        file << "  Position: " << bone.Position.x << " " << bone.Position.y << " " << bone.Position.z << "\n";
        file << "  Rotation: " << bone.Rotation.x << " " << bone.Rotation.y << " " << bone.Rotation.z << " " << bone.Rotation.w << "\n";
        file << "  Scale: " << bone.Scale.x << " " << bone.Scale.y << " " << bone.Scale.z << "\n";
    }
    
    file.close();
    LOG_INFO("Skeleton saved to: {}", filepath);
    return true;
}

std::shared_ptr<Skeleton> SkeletonManager::LoadSkeleton(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        LOG_ERROR("Failed to open skeleton file: {}", filepath);
        return nullptr;
    }
    
    auto skeleton = std::make_shared<Skeleton>();
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.find("Bone:") != std::string::npos) {
            std::istringstream iss(line);
            std::string prefix, boneName;
            iss >> prefix >> boneName;
            
            // Read parent index
            std::getline(file, line);
            std::istringstream parentStream(line);
            std::string parentLabel;
            int32_t parentIndex;
            parentStream >> parentLabel >> parentIndex;
            
            uint32_t boneIndex = skeleton->AddBone(boneName, parentIndex);
            
            // Read position
            std::getline(file, line);
            std::istringstream posStream(line);
            std::string posLabel;
            float x, y, z;
            posStream >> posLabel >> x >> y >> z;
            skeleton->GetBone(boneIndex).Position = Vector3(x, y, z);
            
            // Read rotation
            std::getline(file, line);
            std::istringstream rotStream(line);
            std::string rotLabel;
            float qx, qy, qz, qw;
            rotStream >> rotLabel >> qx >> qy >> qz >> qw;
            skeleton->GetBone(boneIndex).Rotation = Quaternion(qx, qy, qz, qw);
            
            // Read scale
            std::getline(file, line);
            std::istringstream scaleStream(line);
            std::string scaleLabel;
            float sx, sy, sz;
            scaleStream >> scaleLabel >> sx >> sy >> sz;
            skeleton->GetBone(boneIndex).Scale = Vector3(sx, sy, sz);
        }
    }
    
    file.close();
    
    if (ValidateSkeleton(skeleton)) {
        LOG_INFO("Skeleton loaded from: {} ({} bones)", filepath, skeleton->GetBoneCount());
        return skeleton;
    }
    
    return nullptr;
}

std::shared_ptr<Skeleton> SkeletonManager::CloneSkeleton(const std::shared_ptr<Skeleton>& skeleton) {
    if (!skeleton) return nullptr;
    
    auto clonedSkeleton = std::make_shared<Skeleton>();
    
    for (uint32_t i = 0; i < skeleton->GetBoneCount(); ++i) {
        const auto& bone = skeleton->GetBone(i);
        uint32_t newBoneIndex = clonedSkeleton->AddBone(bone.Name, bone.ParentIndex);
        
        auto& newBone = clonedSkeleton->GetBone(newBoneIndex);
        newBone.Position = bone.Position;
        newBone.Rotation = bone.Rotation;
        newBone.Scale = bone.Scale;
    }
    
    LOG_INFO("Skeleton cloned with {} bones", clonedSkeleton->GetBoneCount());
    return clonedSkeleton;
}

bool SkeletonManager::ValidateSkeleton(const std::shared_ptr<Skeleton>& skeleton) const {
    if (!skeleton) {
        LOG_ERROR("Skeleton is null");
        return false;
    }
    
    uint32_t boneCount = skeleton->GetBoneCount();
    if (boneCount == 0) {
        LOG_ERROR("Skeleton has no bones");
        return false;
    }
    
    // Check parent indices are valid
    for (uint32_t i = 0; i < boneCount; ++i) {
        const auto& bone = skeleton->GetBone(i);
        if (bone.ParentIndex >= static_cast<int32_t>(boneCount) && bone.ParentIndex != -1) {
            LOG_ERROR("Bone '{}' has invalid parent index: {}", bone.Name, bone.ParentIndex);
            return false;
        }
    }
    
    return true;
}

} // namespace Luminary
