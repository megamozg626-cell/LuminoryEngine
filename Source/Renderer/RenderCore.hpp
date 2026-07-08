#pragma once

#include <memory>
#include <vector>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "../Math/Vector3.hpp"
#include "../Math/Matrix4.hpp"

namespace Luminary {

class Scene;
class Camera;
class Material;

class Renderer {
public:
    Renderer();
    ~Renderer();

    bool Initialize();
    void Shutdown();

    void BeginFrame();
    void EndFrame();
    void RenderScene(std::shared_ptr<Scene> scene);

    void SetClearColor(float r, float g, float b, float a);
    void EnableVSync(bool enable) { m_VSyncEnabled = enable; }

    uint32_t GetScreenWidth() const { return m_ScreenWidth; }
    uint32_t GetScreenHeight() const { return m_ScreenHeight; }

private:
    bool m_VSyncEnabled = true;
    uint32_t m_ScreenWidth = 1920;
    uint32_t m_ScreenHeight = 1080;
    float m_ClearColor[4] = { 0.1f, 0.1f, 0.1f, 1.0f };

    // DirectX 12 resources
    ID3D12Device* m_Device = nullptr;
    ID3D12CommandQueue* m_CommandQueue = nullptr;
    IDXGISwapChain4* m_SwapChain = nullptr;
    ID3D12CommandAllocator* m_CommandAllocator = nullptr;
    ID3D12GraphicsCommandList* m_CommandList = nullptr;
    ID3D12DescriptorHeap* m_RTVHeap = nullptr;
    ID3D12DescriptorHeap* m_DSVHeap = nullptr;
    ID3D12Resource* m_DepthStencilBuffer = nullptr;
    std::vector<ID3D12Resource*> m_BackBuffers;
    ID3D12Fence* m_Fence = nullptr;
    HANDLE m_FenceEvent = nullptr;
    uint64_t m_FenceValue = 0;

    bool InitializeDirectX12();
    void Present();
};

} // namespace Luminary
