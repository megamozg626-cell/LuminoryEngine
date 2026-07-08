#pragma once

#include <string>
#include <memory>

namespace Luminary {

class AudioSource {
public:
    AudioSource();
    ~AudioSource();

    void SetVolume(float volume) { m_Volume = volume; }
    void SetPitch(float pitch) { m_Pitch = pitch; }
    void SetLoop(bool loop) { m_Loop = loop; }

    float GetVolume() const { return m_Volume; }
    float GetPitch() const { return m_Pitch; }
    bool IsLooping() const { return m_Loop; }

    void Play();
    void Pause();
    void Stop();
    bool IsPlaying() const { return m_IsPlaying; }

private:
    float m_Volume = 1.0f;
    float m_Pitch = 1.0f;
    bool m_Loop = false;
    bool m_IsPlaying = false;
};

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    std::shared_ptr<AudioSource> CreateAudioSource();
    void SetMasterVolume(float volume) { m_MasterVolume = volume; }
    float GetMasterVolume() const { return m_MasterVolume; }

private:
    float m_MasterVolume = 1.0f;
};

} // namespace Luminary
