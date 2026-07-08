#pragma once

#include "../Math/Vector3.hpp"
#include "../Math/Quaternion.hpp"
#include "../Scene/Component.hpp"
#include <memory>

namespace Luminary {

class RigidBody : public Component {
public:
    enum class BodyType {
        Static,
        Dynamic,
        Kinematic
    };

    RigidBody();
    ~RigidBody();

    void SetBodyType(BodyType type) { m_BodyType = type; }
    void SetMass(float mass) { m_Mass = mass > 0 ? mass : 1.0f; }
    void SetGravityScale(float scale) { m_GravityScale = scale; }

    BodyType GetBodyType() const { return m_BodyType; }
    float GetMass() const { return m_Mass; }
    float GetGravityScale() const { return m_GravityScale; }

    void SetVelocity(const Vector3& velocity) { m_Velocity = velocity; }
    void SetAngularVelocity(const Vector3& velocity) { m_AngularVelocity = velocity; }

    const Vector3& GetVelocity() const { return m_Velocity; }
    const Vector3& GetAngularVelocity() const { return m_AngularVelocity; }

    void ApplyForce(const Vector3& force);
    void ApplyImpulse(const Vector3& impulse);
    void ApplyTorque(const Vector3& torque);

    void Update(float deltaTime) override;

private:
    BodyType m_BodyType = BodyType::Dynamic;
    float m_Mass = 1.0f;
    float m_GravityScale = 1.0f;
    Vector3 m_Velocity = Vector3::Zero();
    Vector3 m_AngularVelocity = Vector3::Zero();
    Vector3 m_Force = Vector3::Zero();
    Vector3 m_Torque = Vector3::Zero();
    const Vector3 m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
};

class Collision {
public:
    struct Hit {
        std::shared_ptr<Actor> Actor;
        Vector3 Point;
        Vector3 Normal;
        float Distance = 0.0f;
    };

    static bool RaycastSphere(const Vector3& rayOrigin, const Vector3& rayDirection, 
                             const Vector3& sphereCenter, float sphereRadius, Hit& outHit);
    static bool CheckSphereOverlap(const Vector3& center1, float radius1, 
                                   const Vector3& center2, float radius2);
};

class Physics {
public:
    Physics();
    ~Physics();

    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    void SetGravity(const Vector3& gravity) { m_Gravity = gravity; }
    const Vector3& GetGravity() const { return m_Gravity; }

private:
    Vector3 m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
};

} // namespace Luminary
