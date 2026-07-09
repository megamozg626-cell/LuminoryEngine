#include "AnimationImporter.hpp"
#include "../Core/Logging/Logger.hpp"
#include <fstream>
#include <sstream>

namespace Luminary {

AnimationImporter::AnimationImporter() {
    LOG_DEBUG("AnimationImporter created");
}

AnimationImporter::~AnimationImporter() = default;

std::vector<std::shared_ptr<Animation>> AnimationImporter::ImportAnimationsFromModel(const std::string& filepath, const std::shared_ptr<Skeleton>& targetSkeleton) {
    std::vector<std::shared_ptr<Animation>> animations;
    
    ModelImporter importer;
    ModelImporter::ModelData modelData;
    
    if (!importer.LoadModel(filepath, modelData)) {
        SetError("Failed to load model: " + importer.GetLastError());
        LOG_ERROR("Animation import failed: {}", GetLastError());
        return animations;
    }
    
    LOG_INFO("Importing {} animations from: {}", modelData.Animations.size(), filepath);
    
    for (const auto& animData : modelData.Animations) {
        auto animation = CreateAnimationFromData(animData, targetSkeleton);
        if (animation) {
            animations.push_back(animation);
        }
    }
    
    return animations;
}

std::shared_ptr<Animation> AnimationImporter::ImportAnimation(const std::string& filepath, const std::string& animationName, const std::shared_ptr<Skeleton>& targetSkeleton) {
    ModelImporter importer;
    ModelImporter::ModelData modelData;
    
    if (!importer.LoadModel(filepath, modelData)) {
        SetError("Failed to load model: " + importer.GetLastError());
        return nullptr;
    }
    
    for (const auto& animData : modelData.Animations) {
        if (animData.Name == animationName) {
            return CreateAnimationFromData(animData, targetSkeleton);
        }
    }
    
    SetError("Animation not found: " + animationName);
    return nullptr;
}

std::shared_ptr<Animation> AnimationImporter::CreateAnimationFromData(const ModelImporter::AnimationData& data, const std::shared_ptr<Skeleton>& skeleton) {
    if (!skeleton) {
        SetError("Target skeleton is null");
        LOG_ERROR("Cannot create animation without skeleton");
        return nullptr;
    }
    
    auto animation = std::make_shared<Animation>(data.Name);
    animation->SetDuration(data.Duration);
    
    LOG_DEBUG("Creating animation: {} (duration: {:.2f}s, {} bone anims)",
        data.Name, data.Duration, data.BoneAnimations.size());
    
    for (const auto& boneAnimData : data.BoneAnimations) {
        BoneAnimation boneAnim;
        boneAnim.BoneIndex = boneAnimData.BoneIndex;
        
        // Create keyframes
        for (const auto& keyframe : boneAnimData.KeyFrames) {
            KeyFrame kf;
            kf.Time = keyframe.Time;
            kf.Position = keyframe.Position;
            kf.Rotation = keyframe.Rotation;
            kf.Scale = keyframe.Scale;
            boneAnim.KeyFrames.push_back(kf);
        }
        
        animation->AddBoneAnimation(boneAnim);
    }
    
    if (ValidateAnimation(animation, skeleton)) {
        LOG_INFO("Animation created successfully: {}", data.Name);
        m_AnimationCache[data.Name] = animation;
        return animation;
    }
    
    SetError("Animation validation failed");
    return nullptr;
}

bool AnimationImporter::SaveAnimation(const std::string& filepath, const std::shared_ptr<Animation>& animation) {
    if (!animation) {
        SetError("Animation is null");
        return false;
    }
    
    std::ofstream file(filepath);
    if (!file.is_open()) {
        SetError("Failed to open file for saving: " + filepath);
        LOG_ERROR("Animation save failed: {}", GetLastError());
        return false;
    }
    
    const auto& boneAnims = animation->GetBoneAnimations();
    
    file << "ANIMATION\n";
    file << "Duration: " << animation->GetDuration() << "\n";
    file << "BoneAnimationCount: " << boneAnims.size() << "\n\n";
    
    for (const auto& boneAnim : boneAnims) {
        file << "BoneIndex: " << boneAnim.BoneIndex << "\n";
        file << "KeyFrameCount: " << boneAnim.KeyFrames.size() << "\n";
        
        for (const auto& keyframe : boneAnim.KeyFrames) {
            file << "  Time: " << keyframe.Time << "\n";
            file << "    Position: " << keyframe.Position.x << " " << keyframe.Position.y << " " << keyframe.Position.z << "\n";
            file << "    Rotation: " << keyframe.Rotation.x << " " << keyframe.Rotation.y << " " << keyframe.Rotation.z << " " << keyframe.Rotation.w << "\n";
            file << "    Scale: " << keyframe.Scale.x << " " << keyframe.Scale.y << " " << keyframe.Scale.z << "\n";
        }
    }
    
    file.close();
    LOG_INFO("Animation saved to: {}", filepath);
    return true;
}

std::shared_ptr<Animation> AnimationImporter::LoadAnimation(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        SetError("Failed to open animation file: " + filepath);
        return nullptr;
    }
    
    auto animation = std::make_shared<Animation>("LoadedAnimation");
    std::string line;
    
    // Read duration
    std::getline(file, line);
    std::getline(file, line);
    std::istringstream durationStream(line);
    std::string durationLabel;
    float duration;
    durationStream >> durationLabel >> duration;
    animation->SetDuration(duration);
    
    // Read bone animations
    while (std::getline(file, line)) {
        if (line.find("BoneIndex:") != std::string::npos) {
            std::istringstream iss(line);
            std::string label;
            uint32_t boneIndex;
            iss >> label >> boneIndex;
            
            BoneAnimation boneAnim;
            boneAnim.BoneIndex = boneIndex;
            
            // Read keyframe count
            std::getline(file, line);
            std::istringstream countStream(line);
            std::string countLabel;
            int keyframeCount;
            countStream >> countLabel >> keyframeCount;
            
            // Read keyframes
            for (int i = 0; i < keyframeCount; ++i) {
                KeyFrame keyframe;
                
                // Read time
                std::getline(file, line);
                std::istringstream timeStream(line);
                std::string timeLabel;
                timeStream >> timeLabel >> keyframe.Time;
                
                // Read position
                std::getline(file, line);
                std::istringstream posStream(line);
                std::string posLabel;
                float px, py, pz;
                posStream >> posLabel >> px >> py >> pz;
                keyframe.Position = Vector3(px, py, pz);
                
                // Read rotation
                std::getline(file, line);
                std::istringstream rotStream(line);
                std::string rotLabel;
                float qx, qy, qz, qw;
                rotStream >> rotLabel >> qx >> qy >> qz >> qw;
                keyframe.Rotation = Quaternion(qx, qy, qz, qw);
                
                // Read scale
                std::getline(file, line);
                std::istringstream scaleStream(line);
                std::string scaleLabel;
                float sx, sy, sz;
                scaleStream >> scaleLabel >> sx >> sy >> sz;
                keyframe.Scale = Vector3(sx, sy, sz);
                
                boneAnim.KeyFrames.push_back(keyframe);
            }
            
            animation->AddBoneAnimation(boneAnim);
        }
    }
    
    file.close();
    LOG_INFO("Animation loaded from: {}", filepath);
    return animation;
}

bool AnimationImporter::ValidateAnimation(const std::shared_ptr<Animation>& animation, const std::shared_ptr<Skeleton>& skeleton) const {
    if (!animation || !skeleton) {
        LOG_ERROR("Animation or skeleton is null");
        return false;
    }
    
    const auto& boneAnims = animation->GetBoneAnimations();
    if (boneAnims.empty()) {
        LOG_WARNING("Animation has no bone animations");
        return false;
    }
    
    for (const auto& boneAnim : boneAnims) {
        if (boneAnim.BoneIndex >= skeleton->GetBoneCount()) {
            LOG_ERROR("Animation bone index {} exceeds skeleton bone count {}",
                boneAnim.BoneIndex, skeleton->GetBoneCount());
            return false;
        }
        
        if (boneAnim.KeyFrames.empty()) {
            LOG_WARNING("Bone animation {} has no keyframes", boneAnim.BoneIndex);
            return false;
        }
    }
    
    return true;
}

} // namespace Luminary
