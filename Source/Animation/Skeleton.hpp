#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include <vector>
#include <string>
#include <memory>

namespace Luminary {

struct Bone {
    std::string Name;
    int32_t ParentIndex = -1;
    Vector3 Position = Vector3::Zero();
    Quaternion Rotation = Quaternion::Identity();
    Vector3 Scale = Vector3::One();
};

class Skeleton {
public:
    Skeleton();
    ~Skeleton();

    uint32_t AddBone(const std::string& name, int32_t parentIndex = -1);
    const Bone& GetBone(uint32_t index) const { return m_Bones[index]; }
    Bone& GetBone(uint32_t index) { return m_Bones[index]; }
    uint32_t GetBoneCount() const { return static_cast<uint32_t>(m_Bones.size()); }
    int32_t FindBoneByName(const std::string& name) const;

    // Humanoid Skeleton
    static std::shared_ptr<Skeleton> CreateHumanoidSkeleton();

private:
    std::vector<Bone> m_Bones;
};

struct KeyFrame {
    float Time = 0.0f;
    Vector3 Position = Vector3::Zero();
    Quaternion Rotation = Quaternion::Identity();
    Vector3 Scale = Vector3::One();
};

struct BoneAnimation {
    uint32_t BoneIndex = 0;
    std::vector<KeyFrame> KeyFrames;
};

class Animation {
public:
    Animation(const std::string& name = "Animation");
    ~Animation();

    void SetDuration(float duration) { m_Duration = duration; }
    float GetDuration() const { return m_Duration; }

    void AddBoneAnimation(const BoneAnimation& animation);
    const std::vector<BoneAnimation>& GetBoneAnimations() const { return m_BoneAnimations; }

    void Sample(float time, std::vector<Bone>& outBones) const;

private:
    std::string m_Name;
    float m_Duration = 0.0f;
    std::vector<BoneAnimation> m_BoneAnimations;

    KeyFrame SampleBoneAnimation(const BoneAnimation& animation, float time) const;
};

} // namespace Luminary
