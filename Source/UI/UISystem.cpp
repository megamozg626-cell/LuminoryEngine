#include "UISystem.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

UIElement::UIElement() {
    LOG_DEBUG("UIElement created");
}

UIElement::~UIElement() = default;

Button::Button() {
    LOG_DEBUG("Button created");
    SetSize(Vector2(100.0f, 40.0f));
}

Button::~Button() = default;

void Button::OnClick() {
    if (m_OnClickCallback && m_IsEnabled) {
        m_OnClickCallback();
    }
}

void Button::Draw() {
    if (!m_IsVisible) return;
    LOG_DEBUG("Drawing button: {}", m_Text);
}

Label::Label() {
    LOG_DEBUG("Label created");
}

Label::~Label() = default;

void Label::Draw() {
    if (!m_IsVisible) return;
    LOG_DEBUG("Drawing label: {}", m_Text);
}

TextInput::TextInput() {
    LOG_DEBUG("TextInput created");
    SetSize(Vector2(200.0f, 30.0f));
}

TextInput::~TextInput() = default;

void TextInput::Draw() {
    if (!m_IsVisible) return;
    LOG_DEBUG("Drawing text input");
}

void TextInput::Update(float deltaTime) {
    // Handle input
}

Panel::Panel() {
    LOG_DEBUG("Panel created");
}

Panel::~Panel() = default;

void Panel::AddChild(std::shared_ptr<UIElement> child) {
    if (child) {
        m_Children.push_back(child);
    }
}

void Panel::RemoveChild(std::shared_ptr<UIElement> child) {
    if (child) {
        m_Children.erase(
            std::remove(m_Children.begin(), m_Children.end(), child),
            m_Children.end()
        );
    }
}

void Panel::Draw() {
    if (!m_IsVisible) return;

    for (auto& child : m_Children) {
        if (child->IsVisible()) {
            child->Draw();
        }
    }
}

void Panel::Update(float deltaTime) {
    for (auto& child : m_Children) {
        if (child->IsEnabled()) {
            child->Update(deltaTime);
        }
    }
}

UICanvas::UICanvas() {
    LOG_INFO("UICanvas created");
}

UICanvas::~UICanvas() {
    m_Elements.clear();
}

void UICanvas::AddElement(std::shared_ptr<UIElement> element) {
    if (element) {
        m_Elements.push_back(element);
    }
}

void UICanvas::RemoveElement(std::shared_ptr<UIElement> element) {
    if (element) {
        m_Elements.erase(
            std::remove(m_Elements.begin(), m_Elements.end(), element),
            m_Elements.end()
        );
    }
}

void UICanvas::Update(float deltaTime) {
    for (auto& element : m_Elements) {
        if (element->IsEnabled()) {
            element->Update(deltaTime);
        }
    }
}

void UICanvas::Draw() {
    for (auto& element : m_Elements) {
        if (element->IsVisible()) {
            element->Draw();
        }
    }
}

} // namespace Luminary
