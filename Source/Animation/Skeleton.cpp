#include "Skeleton.hpp"
#include "../Core/Logging/Logger.hpp"
#include <algorithm>

namespace Luminary {

Skeleton::Skeleton() = default;
Skeleton::~Skeleton() = default;

uint32_t Skeleton::AddBone(const std::string& name, int32_t parentIndex) {
    Bone bone;
    bone.Name = name;
    bone.ParentIndex = parentIndex;
    m_Bones.push_back(bone);
    return static_cast<uint32_t>(m_Bones.size() - 1);
}

int32_t Skeleton::FindBoneByName(const std::string& name) const {
    for (size_t i = 0; i < m_Bones.size(); ++i) {
        if (m_Bones[i].Name == name) {
            return static_cast<int32_t>(i);
        }
    }
    return -1;
}

std::shared_ptr<Skeleton> Skeleton::CreateHumanoidSkeleton() {
    auto skeleton = std::make_shared<Skeleton>();

    // Root
    uint32_t root = skeleton->AddBone("Root", -1);
    // Pelvis
    uint32_t pelvis = skeleton->AddBone("Pelvis", root);
    // Spine
    uint32_t spine = skeleton->AddBone("Spine", pelvis);
    uint32_t chest = skeleton->AddBone("Chest", spine);
    uint32_t neck = skeleton->AddBone("Neck", chest);
    uint32_t head = skeleton->AddBone("Head", neck);

    // Left Arm
    uint32_t leftClavicle = skeleton->AddBone("LeftClavicle", chest);
    uint32_t leftShoulder = skeleton->AddBone("LeftShoulder", leftClavicle);
    uint32_t leftUpperArm = skeleton->AddBone("LeftUpperArm", leftShoulder);
    uint32_t leftForearm = skeleton->AddBone("LeftForearm", leftUpperArm);
    uint32_t leftHand = skeleton->AddBone("LeftHand", leftForearm);

    // Right Arm
    uint32_t rightClavicle = skeleton->AddBone("RightClavicle", chest);
    uint32_t rightShoulder = skeleton->AddBone("RightShoulder", rightClavicle);
    uint32_t rightUpperArm = skeleton->AddBone("RightUpperArm", rightShoulder);
    uint32_t rightForearm = skeleton->AddBone("RightForearm", rightUpperArm);
    uint32_t rightHand = skeleton->AddBone("RightHand", rightForearm);

    // Left Leg
    uint32_t leftThigh = skeleton->AddBone("LeftThigh", pelvis);
    uint32_t leftCalf = skeleton->AddBone("LeftCalf", leftThigh);
    uint32_t leftFoot = skeleton->AddBone("LeftFoot", leftCalf);
    uint32_t leftToe = skeleton->AddBone("LeftToe", leftFoot);

    // Right Leg
    uint32_t rightThigh = skeleton->AddBone("RightThigh", pelvis);
    uint32_t rightCalf = skeleton->AddBone("RightCalf", rightThigh);
    uint32_t rightFoot = skeleton->AddBone("RightFoot", rightCalf);
    uint32_t rightToe = skeleton->AddBone("RightToe", rightFoot);

    LOG_INFO("Humanoid skeleton created with {} bones", skeleton->GetBoneCount());
    return skeleton;
}

Animation::Animation(const std::string& name) : m_Name(name) {}
Animation::~Animation() = default;

void Animation::AddBoneAnimation(const BoneAnimation& animation) {
    m_BoneAnimations.push_back(animation);
}

KeyFrame Animation::SampleBoneAnimation(const BoneAnimation& animation, float time) const {
    if (animation.KeyFrames.empty()) {
        return KeyFrame();
    }

    if (time <= animation.KeyFrames.front().Time) {
        return animation.KeyFrames.front();
    }

    if (time >= animation.KeyFrames.back().Time) {
        return animation.KeyFrames.back();
    }

    for (size_t i = 0; i < animation.KeyFrames.size() - 1; ++i) {
        const KeyFrame& kf1 = animation.KeyFrames[i];
        const KeyFrame& kf2 = animation.KeyFrames[i + 1];

        if (time >= kf1.Time && time <= kf2.Time) {
            float interval = kf2.Time - kf1.Time;
            float t = (time - kf1.Time) / interval;

            KeyFrame result;
            result.Time = time;
            result.Position = kf1.Position.Lerp(kf2.Position, t);
            result.Rotation = Quaternion::Slerp(kf1.Rotation, kf2.Rotation, t);
            result.Scale = kf1.Scale.Lerp(kf2.Scale, t);
            return result;
        }
    }

    return animation.KeyFrames.back();
}

void Animation::Sample(float time, std::vector<Bone>& outBones) const {
    for (const auto& boneAnim : m_BoneAnimations) {
        if (boneAnim.BoneIndex < outBones.size()) {
            KeyFrame keyFrame = SampleBoneAnimation(boneAnim, time);
            outBones[boneAnim.BoneIndex].Position = keyFrame.Position;
            outBones[boneAnim.BoneIndex].Rotation = keyFrame.Rotation;
            outBones[boneAnim.BoneIndex].Scale = keyFrame.Scale;
        }
    }
}

} // namespace Luminary
