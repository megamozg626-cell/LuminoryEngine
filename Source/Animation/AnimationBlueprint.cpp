#include "AnimationBlueprint.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

AnimationBlueprint::AnimationBlueprint() {
    LOG_DEBUG("AnimationBlueprint created");
}

AnimationBlueprint::~AnimationBlueprint() = default;

void AnimationBlueprint::AddAnimation(AnimationState state, std::shared_ptr<Animation> animation) {
    if (animation) {
        m_Animations[state] = animation;
    }
}

void AnimationBlueprint::PlayAnimation(AnimationState state) {
    auto it = m_Animations.find(state);
    if (it != m_Animations.end()) {
        m_CurrentState = state;
        m_PlaybackTime = 0.0f;
        m_IsPlaying = true;
    }
}

void AnimationBlueprint::StopAnimation() {
    m_IsPlaying = false;
    m_PlaybackTime = 0.0f;
}

void AnimationBlueprint::Update(float deltaTime) {
    if (!m_IsPlaying || !m_Skeleton) return;

    auto it = m_Animations.find(m_CurrentState);
    if (it == m_Animations.end()) return;

    std::shared_ptr<Animation> animation = it->second;
    m_PlaybackTime += deltaTime;

    if (m_PlaybackTime > animation->GetDuration()) {
        m_PlaybackTime = 0.0f;
    }

    // Sample animation
    std::vector<Bone> bones(m_Skeleton->GetBoneCount());
    for (uint32_t i = 0; i < m_Skeleton->GetBoneCount(); ++i) {
        bones[i] = m_Skeleton->GetBone(i);
    }
    animation->Sample(m_PlaybackTime, bones);
}

AnimationStateMachine::AnimationStateMachine() {
    LOG_DEBUG("AnimationStateMachine created");
}

AnimationStateMachine::~AnimationStateMachine() = default;

void AnimationStateMachine::AddState(const std::string& name, std::shared_ptr<Animation> animation) {
    m_States[name] = {name, animation};
    if (m_CurrentState.empty()) {
        m_CurrentState = name;
    }
}

void AnimationStateMachine::AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition) {
    m_Transitions.push_back({from, to, condition});
}

void AnimationStateMachine::Update(float deltaTime) {
    // Check transitions
    for (const auto& transition : m_Transitions) {
        if (transition.From == m_CurrentState && transition.Condition()) {
            m_CurrentState = transition.To;
            m_PlaybackTime = 0.0f;
            break;
        }
    }

    // Update current state playback time
    auto it = m_States.find(m_CurrentState);
    if (it != m_States.end() && it->second.Animation) {
        m_PlaybackTime += deltaTime;
        if (m_PlaybackTime > it->second.Animation->GetDuration()) {
            m_PlaybackTime = 0.0f;
        }
    }
}

} // namespace Luminary
