#pragma once

#include <string>
#include <vector>
#include <memory>

namespace Luminary {

class Particle {
public:
    struct EmitterSettings {
        float Duration = 1.0f;
        float SpawnRate = 10.0f;
        float InitialVelocity = 5.0f;
        float Gravity = 9.81f;
        float Damping = 0.1f;
    };

    Particle();
    ~Particle();

    void SetPosition(const Vector3& pos) { m_Position = pos; }
    void SetVelocity(const Vector3& vel) { m_Velocity = vel; }
    void SetLifetime(float lifetime) { m_Lifetime = lifetime; m_MaxLifetime = lifetime; }

    const Vector3& GetPosition() const { return m_Position; }
    const Vector3& GetVelocity() const { return m_Velocity; }
    float GetLifetime() const { return m_Lifetime; }
    float GetMaxLifetime() const { return m_MaxLifetime; }

    void Update(float deltaTime);
    bool IsAlive() const { return m_Lifetime > 0.0f; }

private:
    Vector3 m_Position = Vector3::Zero();
    Vector3 m_Velocity = Vector3::Zero();
    float m_Lifetime = 1.0f;
    float m_MaxLifetime = 1.0f;
};

class ParticleEmitter {
public:
    ParticleEmitter();
    ~ParticleEmitter();

    void SetEmitterSettings(const Particle::EmitterSettings& settings) { m_Settings = settings; }
    const Particle::EmitterSettings& GetEmitterSettings() const { return m_Settings; }

    void Start();
    void Stop();
    void Update(float deltaTime);

    const std::vector<std::shared_ptr<Particle>>& GetParticles() const { return m_Particles; }
    size_t GetActiveParticleCount() const;

private:
    Particle::EmitterSettings m_Settings;
    std::vector<std::shared_ptr<Particle>> m_Particles;
    bool m_IsEmitting = false;
    float m_EmissionAccumulator = 0.0f;
};

} // namespace Luminary
