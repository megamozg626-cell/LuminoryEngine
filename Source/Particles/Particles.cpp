#include "Particles.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Math/Vector3.hpp"

namespace Luminary {

Particle::Particle() = default;
Particle::~Particle() = default;

void Particle::Update(float deltaTime) {
    m_Velocity.y -= 9.81f * deltaTime;
    m_Position += m_Velocity * deltaTime;
    m_Lifetime -= deltaTime;
}

ParticleEmitter::ParticleEmitter() {
    LOG_DEBUG("ParticleEmitter created");
}

ParticleEmitter::~ParticleEmitter() = default;

void ParticleEmitter::Start() {
    m_IsEmitting = true;
    LOG_DEBUG("ParticleEmitter started");
}

void ParticleEmitter::Stop() {
    m_IsEmitting = false;
    LOG_DEBUG("ParticleEmitter stopped");
}

void ParticleEmitter::Update(float deltaTime) {
    if (m_IsEmitting) {
        m_EmissionAccumulator += deltaTime;
        float particlesPerDeltaTime = m_Settings.SpawnRate * deltaTime;
        m_EmissionAccumulator -= static_cast<float>(static_cast<int32_t>(particlesPerDeltaTime));

        for (int32_t i = 0; i < static_cast<int32_t>(particlesPerDeltaTime); ++i) {
            auto particle = std::make_shared<Particle>();
            particle->SetLifetime(m_Settings.Duration);
            particle->SetVelocity(Vector3::Zero());
            m_Particles.push_back(particle);
        }
    }

    // Update particles
    for (auto& particle : m_Particles) {
        particle->Update(deltaTime);
    }

    // Remove dead particles
    m_Particles.erase(
        std::remove_if(m_Particles.begin(), m_Particles.end(),
            [](const std::shared_ptr<Particle>& p) { return !p->IsAlive(); }),
        m_Particles.end()
    );
}

size_t ParticleEmitter::GetActiveParticleCount() const {
    return m_Particles.size();
}

} // namespace Luminary
