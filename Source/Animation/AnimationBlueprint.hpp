#pragma once

#include "Skeleton.hpp"
#include "../Scene/Component.hpp"
#include "../Math/Vector3.hpp"
#include <memory>
#include <map>

namespace Luminary {

class AnimationBlueprint : public Component {
public:
    enum class AnimationState {
        Idle,
        Walk,
        Run,
        Sprint,
        Jump,
        Fall,
        Land,
        TurnLeft,
        TurnRight,
        WalkBack,
        StrafeLeft,
        StrafeRight
    };

    AnimationBlueprint();
    ~AnimationBlueprint();

    void SetSkeleton(std::shared_ptr<Skeleton> skeleton) { m_Skeleton = skeleton; }
    void AddAnimation(AnimationState state, std::shared_ptr<Animation> animation);
    void PlayAnimation(AnimationState state);
    void StopAnimation();
    void Update(float deltaTime) override;

    AnimationState GetCurrentState() const { return m_CurrentState; }
    float GetPlaybackTime() const { return m_PlaybackTime; }

private:
    std::shared_ptr<Skeleton> m_Skeleton;
    std::map<AnimationState, std::shared_ptr<Animation>> m_Animations;
    AnimationState m_CurrentState = AnimationState::Idle;
    float m_PlaybackTime = 0.0f;
    bool m_IsPlaying = false;
};

class AnimationStateMachine {
public:
    AnimationStateMachine();
    ~AnimationStateMachine();

    void AddState(const std::string& name, std::shared_ptr<Animation> animation);
    void AddTransition(const std::string& from, const std::string& to, std::function<bool()> condition);
    void Update(float deltaTime);

private:
    struct State {
        std::string Name;
        std::shared_ptr<Animation> Animation;
    };

    struct Transition {
        std::string From;
        std::string To;
        std::function<bool()> Condition;
    };

    std::map<std::string, State> m_States;
    std::vector<Transition> m_Transitions;
    std::string m_CurrentState;
    float m_PlaybackTime = 0.0f;
};

} // namespace Luminary
