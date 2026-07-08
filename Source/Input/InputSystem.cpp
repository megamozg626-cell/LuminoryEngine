#include "InputSystem.hpp"
#include "../Core/Logging/Logger.hpp"
#include <Windows.h>

namespace Luminary {

InputSystem::InputSystem() {
    LOG_DEBUG("InputSystem created");
}

InputSystem::~InputSystem() {
    Shutdown();
}

bool InputSystem::Initialize() {
    LOG_INFO("InputSystem initialized");
    return true;
}

void InputSystem::Shutdown() {
    LOG_INFO("InputSystem shut down");
}

void InputSystem::ProcessInput() {
    m_LastMousePosition = m_MousePosition;
    m_MouseDelta = Vector2::Zero();
    m_MouseWheel = 0.0f;

    // Clear pressed and released states
    for (auto& [key, pressed] : m_KeyPressed) {
        pressed = false;
    }
    for (auto& [key, released] : m_KeyReleased) {
        released = false;
    }

    // Process Windows messages
    for (int i = 0; i < 256; ++i) {
        short keyState = GetAsyncKeyState(i);
        bool isPressed = (keyState & 0x8000) != 0;
        KeyCode key = static_cast<KeyCode>(i);

        bool wasHeld = m_KeyHeld[key];
        m_KeyHeld[key] = isPressed;

        if (isPressed && !wasHeld) {
            m_KeyPressed[key] = true;
        } else if (!isPressed && wasHeld) {
            m_KeyReleased[key] = true;
        }
    }

    // Get mouse position
    POINT mousePos;
    GetCursorPos(&mousePos);
    m_MousePosition = Vector2(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
    m_MouseDelta = m_MousePosition - m_LastMousePosition;
}

bool InputSystem::IsKeyPressed(KeyCode key) const {
    auto it = m_KeyPressed.find(key);
    return it != m_KeyPressed.end() && it->second;
}

bool InputSystem::IsKeyHeld(KeyCode key) const {
    auto it = m_KeyHeld.find(key);
    return it != m_KeyHeld.end() && it->second;
}

bool InputSystem::IsKeyReleased(KeyCode key) const {
    auto it = m_KeyReleased.find(key);
    return it != m_KeyReleased.end() && it->second;
}

bool InputSystem::IsMouseButtonPressed(KeyCode button) const {
    return IsKeyPressed(button);
}

void InputSystem::RegisterKeyCallback(KeyCode key, InputType type, std::function<void()> callback) {
    m_Callbacks[{key, type}].push_back(callback);
}

void InputSystem::RegisterAxisMapping(const std::string& name, KeyCode negativeKey, KeyCode positiveKey) {
    m_AxisMappings[name] = {negativeKey, positiveKey};
}

float InputSystem::GetAxis(const std::string& name) const {
    auto it = m_AxisMappings.find(name);
    if (it == m_AxisMappings.end()) return 0.0f;

    float axis = 0.0f;
    if (IsKeyHeld(it->second.first)) axis -= 1.0f;
    if (IsKeyHeld(it->second.second)) axis += 1.0f;
    return axis;
}

} // namespace Luminary
