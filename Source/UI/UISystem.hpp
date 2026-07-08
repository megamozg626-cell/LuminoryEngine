#pragma once

#include "../Math/Vector2.hpp"
#include "../Math/Vector3.hpp"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Luminary {

class UIElement {
public:
    enum class HorizontalAlignment {
        Left,
        Center,
        Right,
        Stretch
    };

    enum class VerticalAlignment {
        Top,
        Center,
        Bottom,
        Stretch
    };

    UIElement();
    virtual ~UIElement();

    void SetPosition(const Vector2& position) { m_Position = position; }
    void SetSize(const Vector2& size) { m_Size = size; }
    void SetVisible(bool visible) { m_IsVisible = visible; }
    void SetEnabled(bool enabled) { m_IsEnabled = enabled; }

    const Vector2& GetPosition() const { return m_Position; }
    const Vector2& GetSize() const { return m_Size; }
    bool IsVisible() const { return m_IsVisible; }
    bool IsEnabled() const { return m_IsEnabled; }

    void SetHorizontalAlignment(HorizontalAlignment alignment) { m_HorizontalAlignment = alignment; }
    void SetVerticalAlignment(VerticalAlignment alignment) { m_VerticalAlignment = alignment; }

    virtual void Update(float deltaTime) {}
    virtual void Draw() {}
    virtual void OnMouseEnter() {}
    virtual void OnMouseExit() {}
    virtual void OnClick() {}

protected:
    Vector2 m_Position = Vector2::Zero();
    Vector2 m_Size = Vector2(100.0f, 100.0f);
    bool m_IsVisible = true;
    bool m_IsEnabled = true;
    HorizontalAlignment m_HorizontalAlignment = HorizontalAlignment::Left;
    VerticalAlignment m_VerticalAlignment = VerticalAlignment::Top;
};

class Button : public UIElement {
public:
    Button();
    ~Button();

    void SetText(const std::string& text) { m_Text = text; }
    const std::string& GetText() const { return m_Text; }

    void SetOnClickCallback(std::function<void()> callback) { m_OnClickCallback = callback; }
    void OnClick() override;

    void Draw() override;

private:
    std::string m_Text;
    std::function<void()> m_OnClickCallback;
    bool m_IsHovered = false;
};

class Label : public UIElement {
public:
    Label();
    ~Label();

    void SetText(const std::string& text) { m_Text = text; }
    const std::string& GetText() const { return m_Text; }

    void Draw() override;

private:
    std::string m_Text;
};

class TextInput : public UIElement {
public:
    TextInput();
    ~TextInput();

    void SetText(const std::string& text) { m_Text = text; }
    const std::string& GetText() const { return m_Text; }

    void SetMaxLength(uint32_t maxLength) { m_MaxLength = maxLength; }
    void SetPlaceholder(const std::string& placeholder) { m_Placeholder = placeholder; }

    void Draw() override;
    void Update(float deltaTime) override;

private:
    std::string m_Text;
    std::string m_Placeholder;
    uint32_t m_MaxLength = 256;
    bool m_IsFocused = false;
};

class Panel : public UIElement {
public:
    Panel();
    ~Panel();

    void AddChild(std::shared_ptr<UIElement> child);
    void RemoveChild(std::shared_ptr<UIElement> child);
    const std::vector<std::shared_ptr<UIElement>>& GetChildren() const { return m_Children; }

    void Draw() override;
    void Update(float deltaTime) override;

private:
    std::vector<std::shared_ptr<UIElement>> m_Children;
};

class UICanvas {
public:
    UICanvas();
    ~UICanvas();

    void AddElement(std::shared_ptr<UIElement> element);
    void RemoveElement(std::shared_ptr<UIElement> element);

    void Update(float deltaTime);
    void Draw();

    const std::vector<std::shared_ptr<UIElement>>& GetElements() const { return m_Elements; }

private:
    std::vector<std::shared_ptr<UIElement>> m_Elements;
};

} // namespace Luminary
