#pragma once

#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include <map>
#include <functional>
#include <memory>
#include <string>

namespace Luminary {

class InputSystem {
public:
    enum class KeyCode {
        A = 0x41, B = 0x42, C = 0x43, D = 0x44, E = 0x45, F = 0x46, G = 0x47, H = 0x48,
        I = 0x49, J = 0x4A, K = 0x4B, L = 0x4C, M = 0x4D, N = 0x4E, O = 0x4F, P = 0x50,
        Q = 0x51, R = 0x52, S = 0x53, T = 0x54, U = 0x55, V = 0x56, W = 0x57, X = 0x58,
        Y = 0x59, Z = 0x5A,
        
        Num0 = 0x30, Num1 = 0x31, Num2 = 0x32, Num3 = 0x33, Num4 = 0x34,
        Num5 = 0x35, Num6 = 0x36, Num7 = 0x37, Num8 = 0x38, Num9 = 0x39,
        
        Space = 0x20, Return = 0x0D, Escape = 0x1B, Backspace = 0x08, Tab = 0x09,
        LShift = 0xA0, RShift = 0xA1, LCtrl = 0xA2, RCtrl = 0xA3, LAlt = 0xA4, RAlt = 0xA5,
        
        Left = 0x25, Up = 0x26, Right = 0x27, Down = 0x28,
        Delete = 0x2E, Insert = 0x2D,
        
        F1 = 0x70, F2 = 0x71, F3 = 0x72, F4 = 0x73, F5 = 0x74, F6 = 0x75,
        F7 = 0x76, F8 = 0x77, F9 = 0x78, F10 = 0x79, F11 = 0x7A, F12 = 0x7B,
        
        MouseLeft = 0x01, MouseRight = 0x02, MouseMiddle = 0x04
    };

    enum class InputType {
        Pressed,
        Released,
        Held
    };

    InputSystem();
    ~InputSystem();

    bool Initialize();
    void Shutdown();
    void ProcessInput();

    bool IsKeyPressed(KeyCode key) const;
    bool IsKeyHeld(KeyCode key) const;
    bool IsKeyReleased(KeyCode key) const;
    bool IsMouseButtonPressed(KeyCode button) const;

    Vector2 GetMousePosition() const { return m_MousePosition; }
    Vector2 GetMouseDelta() const { return m_MouseDelta; }
    float GetMouseWheel() const { return m_MouseWheel; }

    void RegisterKeyCallback(KeyCode key, InputType type, std::function<void()> callback);
    void RegisterAxisMapping(const std::string& name, KeyCode negativeKey, KeyCode positiveKey);
    float GetAxis(const std::string& name) const;

private:
    std::map<KeyCode, bool> m_KeyPressed;
    std::map<KeyCode, bool> m_KeyHeld;
    std::map<KeyCode, bool> m_KeyReleased;
    Vector2 m_MousePosition = Vector2::Zero();
    Vector2 m_MouseDelta = Vector2::Zero();
    float m_MouseWheel = 0.0f;
    Vector2 m_LastMousePosition = Vector2::Zero();

    std::map<std::pair<KeyCode, InputType>, std::vector<std::function<void()>>> m_Callbacks;
    std::map<std::string, std::pair<KeyCode, KeyCode>> m_AxisMappings;
};

} // namespace Luminary
