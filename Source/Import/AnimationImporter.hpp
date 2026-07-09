#pragma once

#include "ModelImporter.hpp"
#include "../Animation/Skeleton.hpp"
#include "../Animation/AnimationBlueprint.hpp"
#include <memory>
#include <map>
#include <string>

namespace Luminary {

class AnimationImporter {
public:
    AnimationImporter();
    ~AnimationImporter();

    // Import animations from model file
    std::vector<std::shared_ptr<Animation>> ImportAnimationsFromModel(const std::string& filepath, const std::shared_ptr<Skeleton>& targetSkeleton);
    
    // Import specific animation
    std::shared_ptr<Animation> ImportAnimation(const std::string& filepath, const std::string& animationName, const std::shared_ptr<Skeleton>& targetSkeleton);
    
    // Create animation from keyframe data
    std::shared_ptr<Animation> CreateAnimationFromData(const ModelImporter::AnimationData& data, const std::shared_ptr<Skeleton>& skeleton);
    
    // Save animation to file
    bool SaveAnimation(const std::string& filepath, const std::shared_ptr<Animation>& animation);
    
    // Load animation from file
    std::shared_ptr<Animation> LoadAnimation(const std::string& filepath);
    
    // Animation utilities
    bool ValidateAnimation(const std::shared_ptr<Animation>& animation, const std::shared_ptr<Skeleton>& skeleton) const;
    std::string GetLastError() const { return m_LastError; }

private:
    std::string m_LastError;
    std::map<std::string, std::shared_ptr<Animation>> m_AnimationCache;
    
    void SetError(const std::string& error) { m_LastError = error; }
};

} // namespace Luminary
