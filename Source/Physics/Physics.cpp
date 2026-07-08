#include "Physics.hpp"
#include "../Core/Logging/Logger.hpp"
#include "../Scene/Actor.hpp"
#include <cmath>

namespace Luminary {

RigidBody::RigidBody() = default;
RigidBody::~RigidBody() = default;

void RigidBody::ApplyForce(const Vector3& force) {
    if (m_BodyType == BodyType::Dynamic) {
        m_Force += force;
    }
}

void RigidBody::ApplyImpulse(const Vector3& impulse) {
    if (m_BodyType == BodyType::Dynamic) {
        m_Velocity += impulse / m_Mass;
    }
}

void RigidBody::ApplyTorque(const Vector3& torque) {
    if (m_BodyType == BodyType::Dynamic) {
        m_Torque += torque;
    }
}

void RigidBody::Update(float deltaTime) {
    if (m_BodyType == BodyType::Static) return;

    Vector3 acceleration = m_Force / m_Mass + m_Gravity * m_GravityScale;
    m_Velocity += acceleration * deltaTime;
    m_Force = Vector3::Zero();

    if (auto owner = GetOwner().lock()) {
        auto transform = owner->GetTransform();
        Vector3 newPosition = transform->GetPosition() + m_Velocity * deltaTime;
        transform->SetPosition(newPosition);
    }
}

bool Collision::RaycastSphere(const Vector3& rayOrigin, const Vector3& rayDirection,
                              const Vector3& sphereCenter, float sphereRadius, Hit& outHit) {
    Vector3 oc = rayOrigin - sphereCenter;
    float a = rayDirection.Dot(rayDirection);
    float b = 2.0f * oc.Dot(rayDirection);
    float c = oc.Dot(oc) - sphereRadius * sphereRadius;
    float discriminant = b * b - 4.0f * a * c;

    if (discriminant < 0.0f) {
        return false;
    }

    float sqrtDisc = std::sqrt(discriminant);
    float t = (-b - sqrtDisc) / (2.0f * a);

    if (t < 0.0f) {
        t = (-b + sqrtDisc) / (2.0f * a);
    }

    if (t >= 0.0f) {
        outHit.Point = rayOrigin + rayDirection * t;
        outHit.Normal = (outHit.Point - sphereCenter).Normalize();
        outHit.Distance = t;
        return true;
    }

    return false;
}

bool Collision::CheckSphereOverlap(const Vector3& center1, float radius1,
                                   const Vector3& center2, float radius2) {
    Vector3 diff = center2 - center1;
    float distSquared = diff.LengthSquared();
    float radiusSum = radius1 + radius2;
    return distSquared <= radiusSum * radiusSum;
}

Physics::Physics() {
    LOG_INFO("Physics system created");
}

Physics::~Physics() {
    Shutdown();
}

bool Physics::Initialize() {
    LOG_INFO("Physics system initialized");
    return true;
}

void Physics::Shutdown() {
    LOG_INFO("Physics system shut down");
}

void Physics::Update(float deltaTime) {
    // Physics update will be handled by RigidBody components
}

} // namespace Luminary
