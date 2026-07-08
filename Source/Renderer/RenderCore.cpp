#include "RenderCore.hpp"
#include "../Core/Logging/Logger.hpp"
#include <stdexcept>

namespace Luminary {

Renderer::Renderer() {
    LOG_INFO("Renderer created");
}

Renderer::~Renderer() {
    Shutdown();
}

bool Renderer::Initialize() {
    LOG_INFO("Initializing Renderer...");
    
    try {
        if (!InitializeDirectX12()) {
            LOG_ERROR("Failed to initialize DirectX 12");
            return false;
        }
        LOG_INFO("Renderer initialized successfully");
        return true;
    }
    catch (const std::exception& e) {
        LOG_ERROR("Renderer initialization error: {}", e.what());
        return false;
    }
}

bool Renderer::InitializeDirectX12() {
    LOG_DEBUG("Initializing DirectX 12...");

    // Enable debug layer in debug builds
#ifdef _DEBUG
    ID3D12Debug* debugController = nullptr;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        debugController->EnableDebugLayer();
        debugController->Release();
    }
#endif

    // Create factory
    IDXGIFactory4* factory = nullptr;
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&factory)))) {
        LOG_ERROR("Failed to create DXGI factory");
        return false;
    }

    // Find adapter
    IDXGIAdapter1* adapter = nullptr;
    for (UINT i = 0; SUCCEEDED(factory->EnumAdapters1(i, &adapter)); ++i) {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);
        LOG_DEBUG("Found adapter: {}", static_cast<const char*>(desc.Description));
        break;
    }

    // Create device
    if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&m_Device)))) {
        LOG_ERROR("Failed to create D3D12 device");
        if (adapter) adapter->Release();
        if (factory) factory->Release();
        return false;
    }

    // Create command queue
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    if (FAILED(m_Device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_CommandQueue)))) {
        LOG_ERROR("Failed to create command queue");
        return false;
    }

    // Create swap chain
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount = 2;
    swapChainDesc.Width = m_ScreenWidth;
    swapChainDesc.Height = m_ScreenHeight;
    swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.SampleDesc.Count = 1;

    IDXGISwapChain1* swapChain = nullptr;
    if (FAILED(factory->CreateSwapChainForHwnd(m_CommandQueue, nullptr, &swapChainDesc, nullptr, nullptr, &swapChain))) {
        LOG_ERROR("Failed to create swap chain");
        return false;
    }

    if (FAILED(swapChain->QueryInterface(IID_PPV_ARGS(&m_SwapChain)))) {
        LOG_ERROR("Failed to query swap chain interface");
        swapChain->Release();
        return false;
    }
    swapChain->Release();

    // Create descriptor heaps
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    if (FAILED(m_Device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_RTVHeap)))) {
        LOG_ERROR("Failed to create RTV heap");
        return false;
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    if (FAILED(m_Device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_DSVHeap)))) {
        LOG_ERROR("Failed to create DSV heap");
        return false;
    }

    // Create frame resources
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
    uint32_t rtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

    for (UINT i = 0; i < 2; ++i) {
        ID3D12Resource* backBuffer = nullptr;
        if (FAILED(m_SwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)))) {
            LOG_ERROR("Failed to get back buffer {}", i);
            return false;
        }
        m_BackBuffers.push_back(backBuffer);
        m_Device->CreateRenderTargetView(backBuffer, nullptr, rtvHandle);
        rtvHandle.ptr += rtvDescriptorSize;
    }

    // Create command allocator and list
    if (FAILED(m_Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_CommandAllocator)))) {
        LOG_ERROR("Failed to create command allocator");
        return false;
    }

    if (FAILED(m_Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_CommandAllocator, nullptr, IID_PPV_ARGS(&m_CommandList)))) {
        LOG_ERROR("Failed to create command list");
        return false;
    }

    m_CommandList->Close();

    // Create fence
    if (FAILED(m_Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_Fence)))) {
        LOG_ERROR("Failed to create fence");
        return false;
    }

    m_FenceEvent = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
    if (!m_FenceEvent) {
        LOG_ERROR("Failed to create fence event");
        return false;
    }

    if (adapter) adapter->Release();
    if (factory) factory->Release();

    LOG_DEBUG("DirectX 12 initialized successfully");
    return true;
}

void Renderer::Shutdown() {
    LOG_INFO("Shutting down Renderer");

    // Wait for GPU
    if (m_CommandQueue && m_Fence && m_FenceEvent) {
        m_CommandQueue->Signal(m_Fence, ++m_FenceValue);
        if (m_Fence->GetCompletedValue() < m_FenceValue) {
            m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);
            WaitForSingleObject(m_FenceEvent, INFINITE);
        }
    }

    if (m_FenceEvent) {
        CloseHandle(m_FenceEvent);
        m_FenceEvent = nullptr;
    }

    if (m_Fence) m_Fence->Release();
    if (m_CommandList) m_CommandList->Release();
    if (m_CommandAllocator) m_CommandAllocator->Release();
    if (m_RTVHeap) m_RTVHeap->Release();
    if (m_DSVHeap) m_DSVHeap->Release();
    if (m_DepthStencilBuffer) m_DepthStencilBuffer->Release();
    
    for (auto backBuffer : m_BackBuffers) {
        if (backBuffer) backBuffer->Release();
    }
    m_BackBuffers.clear();

    if (m_SwapChain) m_SwapChain->Release();
    if (m_CommandQueue) m_CommandQueue->Release();
    if (m_Device) m_Device->Release();

    LOG_INFO("Renderer shut down");
}

void Renderer::BeginFrame() {
    if (!m_CommandAllocator || !m_CommandList) return;
    
    m_CommandAllocator->Reset();
    m_CommandList->Reset(m_CommandAllocator, nullptr);
}

void Renderer::EndFrame() {
    if (!m_CommandList) return;
    
    m_CommandList->Close();
    ID3D12CommandList* commandLists[] = { m_CommandList };
    m_CommandQueue->ExecuteCommandLists(1, commandLists);
    
    Present();
}

void Renderer::RenderScene(std::shared_ptr<Scene> scene) {
    if (!scene || !m_CommandList) return;
    
    // Get current back buffer
    UINT currentBackBufferIndex = m_SwapChain ? m_SwapChain->GetCurrentBackBufferIndex() : 0;
    if (currentBackBufferIndex >= m_BackBuffers.size()) return;

    ID3D12Resource* backBuffer = m_BackBuffers[currentBackBufferIndex];
    if (!backBuffer) return;

    // Transition back buffer to render target
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = backBuffer;
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
    m_CommandList->ResourceBarrier(1, &barrier);

    // Clear render target
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_RTVHeap->GetCPUDescriptorHandleForHeapStart();
    uint32_t rtvDescriptorSize = m_Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    rtvHandle.ptr += currentBackBufferIndex * rtvDescriptorSize;

    m_CommandList->ClearRenderTargetView(rtvHandle, m_ClearColor, 0, nullptr);
    m_CommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // Set viewport and scissor rect
    D3D12_VIEWPORT viewport = {};
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;
    viewport.Width = static_cast<float>(m_ScreenWidth);
    viewport.Height = static_cast<float>(m_ScreenHeight);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    m_CommandList->RSSetViewports(1, &viewport);

    D3D12_RECT scissorRect = {};
    scissorRect.right = m_ScreenWidth;
    scissorRect.bottom = m_ScreenHeight;
    m_CommandList->RSSetScissorRects(1, &scissorRect);

    // Transition back buffer to present
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
    m_CommandList->ResourceBarrier(1, &barrier);
}

void Renderer::SetClearColor(float r, float g, float b, float a) {
    m_ClearColor[0] = r;
    m_ClearColor[1] = g;
    m_ClearColor[2] = b;
    m_ClearColor[3] = a;
}

void Renderer::Present() {
    if (m_SwapChain) {
        m_SwapChain->Present(m_VSyncEnabled ? 1 : 0, 0);
    }

    m_CommandQueue->Signal(m_Fence, ++m_FenceValue);
    if (m_Fence->GetCompletedValue() < m_FenceValue) {
        m_Fence->SetEventOnCompletion(m_FenceValue, m_FenceEvent);
        WaitForSingleObject(m_FenceEvent, INFINITE);
    }
}

} // namespace Luminary
