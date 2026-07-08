#include "AudioSystem.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

AudioSource::AudioSource() {
    LOG_DEBUG("AudioSource created");
}

AudioSource::~AudioSource() = default;

void AudioSource::Play() {
    m_IsPlaying = true;
    LOG_DEBUG("AudioSource playing");
}

void AudioSource::Pause() {
    m_IsPlaying = false;
    LOG_DEBUG("AudioSource paused");
}

void AudioSource::Stop() {
    m_IsPlaying = false;
    LOG_DEBUG("AudioSource stopped");
}

AudioSystem::AudioSystem() {
    LOG_INFO("AudioSystem created");
}

AudioSystem::~AudioSystem() {
    Shutdown();
}

bool AudioSystem::Initialize() {
    LOG_INFO("AudioSystem initialized");
    return true;
}

void AudioSystem::Shutdown() {
    LOG_INFO("AudioSystem shut down");
}

void AudioSystem::Update(float deltaTime) {
    // Update audio sources
}

std::shared_ptr<AudioSource> AudioSystem::CreateAudioSource() {
    return std::make_shared<AudioSource>();
}

} // namespace Luminary
