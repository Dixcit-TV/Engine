#include "DX12RHI.h"
#include <DirectXColors.h>
#include <D3DCompiler.h>
#include <format>
#include "../../App/Viewport.h"
#include "../../App/WindowInterface.h"
#include "DX12Resource.h"
#include "../RHIResource.h"
#include "../../Renderer/RenderResource.h"
#include "DX12Shader.h"

DX12RHI::DX12RHI(const Viewport& inViewport, const WindowInterface* const inWindow)
{
    UINT dxgiFactoryFlags = 0;
    //Enable debug layer if needed
#if defined(DEBUG) || defined(_DEBUG)
    {
        ComPtr<ID3D12Debug> DebugController;
        ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&DebugController)));
        DebugController->EnableDebugLayer();

        dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
    }
#endif

    ThrowIfFailed(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(DxgiFactory.ReleaseAndGetAddressOf())));

    //Create Device, nullptr adapter since we use the first/default one
    ThrowIfFailed(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(D3dDevice.ReleaseAndGetAddressOf())));

    //Create Fence used to sync CPU and GPU
    ThrowIfFailed(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(RenderFrameFence.ReleaseAndGetAddressOf())));

    //Store Hardware decriptor sizes for each types
    RtvDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    DsvDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    CbvSrvDescriptorSize = D3dDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;

    //Check for 4x MSAA
    if (MsaaEnabled)
    {
        D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQuality;
        msQuality.Format = backBufferFormat;
        msQuality.SampleCount = 4;
        msQuality.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
        msQuality.NumQualityLevels = 0;
        ThrowIfFailed(D3dDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQuality, sizeof(msQuality)));
        Msaa4xQuality = msQuality.NumQualityLevels;
    }

    //Create Command Queue
    D3D12_COMMAND_QUEUE_DESC queueDesc;
    queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.NodeMask = 0;
    queueDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL;
    ThrowIfFailed(D3dDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(D3dCmdQueue.ReleaseAndGetAddressOf())));

    //Create command list allocator/storage
    ThrowIfFailed(D3dDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(D3dCmdAlloc.ReleaseAndGetAddressOf())));

    //Create command list
    ThrowIfFailed(D3dDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, D3dCmdAlloc.Get(), nullptr, IID_PPV_ARGS(D3dCmdList.ReleaseAndGetAddressOf())));
    D3dCmdList->Close();

    //Creater SwapChain and BackBuffer
    DxgiSwapChain.Reset();

    DXGI_SWAP_CHAIN_DESC swapChainDesc;
    swapChainDesc.BufferCount = 2;
    swapChainDesc.BufferDesc.Width = inViewport.Width;
    swapChainDesc.BufferDesc.Height = inViewport.Height;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.BufferDesc.Format = backBufferFormat;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.Windowed = true;
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    swapChainDesc.OutputWindow = (HWND)inWindow->GetHandle();
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ThrowIfFailed(DxgiFactory->CreateSwapChain(D3dCmdQueue.Get(), &swapChainDesc, &DxgiSwapChain));

    //Create descriptors
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc;
    rtvHeapDesc.NodeMask = 0;
    rtvHeapDesc.NumDescriptors = 2;
    rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(D3dDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(RtvHeapDescriptor.ReleaseAndGetAddressOf())));

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc;
    dsvHeapDesc.NodeMask = 0;
    dsvHeapDesc.NumDescriptors = 1;
    dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    ThrowIfFailed(D3dDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(DsvHeapDescriptor.ReleaseAndGetAddressOf())));

    //Create render targets view for back buffer
    for (UINT idx = 0; idx < 2; ++idx)
    {
        ThrowIfFailed(DxgiSwapChain->GetBuffer(idx, IID_PPV_ARGS(BackBuffers[idx].GetAddressOf())));

        D3dDevice->CreateRenderTargetView(BackBuffers[idx].Get(), nullptr, GetBackBufferHandle(idx));
    }

    //Create Depth/Stencil texture
    D3D12_RESOURCE_DESC dsResourceDesc;
    dsResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    dsResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsResourceDesc.MipLevels = 1;
    dsResourceDesc.Alignment = 0;
    dsResourceDesc.DepthOrArraySize = 1;
    dsResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
    dsResourceDesc.Width = inViewport.Width;
    dsResourceDesc.Height = inViewport.Height;
    dsResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
    dsResourceDesc.SampleDesc.Count = MsaaEnabled ? 4 : 1;
    dsResourceDesc.SampleDesc.Quality = MsaaEnabled ? Msaa4xQuality - 1 : 0;

    D3D12_CLEAR_VALUE dsClearvalue;
    dsClearvalue.Format = dsResourceDesc.Format;
    dsClearvalue.DepthStencil.Depth = 1.f;
    dsClearvalue.DepthStencil.Stencil = 0;

    D3D12_HEAP_PROPERTIES dsHeapProps;
    dsHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;
    dsHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
    dsHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
    dsHeapProps.CreationNodeMask = 0;
    dsHeapProps.VisibleNodeMask = 0;

    ThrowIfFailed(
        D3dDevice->CreateCommittedResource(
            &dsHeapProps
            , D3D12_HEAP_FLAG_NONE
            , &dsResourceDesc
            , D3D12_RESOURCE_STATE_DEPTH_WRITE
            , &dsClearvalue
            , IID_PPV_ARGS(DepthStencilResource.ReleaseAndGetAddressOf())));

    D3dDevice->CreateDepthStencilView(DepthStencilResource.Get(), nullptr, GetDepthStencilHandle());

    //Set Viewport
    D3dViewport.TopLeftX = static_cast<float>(inViewport.Left);
    D3dViewport.TopLeftY = static_cast<float>(inViewport.Top);
    D3dViewport.Width = static_cast<float>(inViewport.Width);
    D3dViewport.Height = static_cast<float>(inViewport.Height);
    D3dViewport.MinDepth = inViewport.MinDepth;
    D3dViewport.MaxDepth = inViewport.MaxDepth;

    //Set ScissorRect
    D3dScissorRect.left = 0;
    D3dScissorRect.top = 0;
    D3dScissorRect.right = static_cast<long>(inViewport.Width);
    D3dScissorRect.bottom = static_cast<long>(inViewport.Height);
}

DX12RHI::~DX12RHI()
{
    FlushCommandQueue();

    D3dCmdList = nullptr;
    D3dCmdAlloc = nullptr;
    D3dCmdQueue = nullptr;
    RenderFrameFence = nullptr;

    DsvHeapDescriptor = nullptr;
    RtvHeapDescriptor = nullptr;

    DepthStencilResource = nullptr;
    for (UINT Idx = 0; Idx < 2; ++Idx)
    {
        BackBuffers[Idx] = nullptr;
    }

    DxgiSwapChain = nullptr;
    D3dDevice = nullptr;
    DxgiFactory = nullptr;
}

void DX12RHI::CompileShader(ShaderInterface* inShader) const
{
	DX12Shader* Dx12Shader = new DX12Shader();

    UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)
    compileFlags |= D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

    std::string target = std::format("{}_5_0", DX12Shader::TypeToShortTypeName(inShader->GetShaderType()));

    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
    HRESULT hr = D3DCompileFromFile(inShader->GetShaderFilePath().c_str(), nullptr, nullptr, inShader->GetShaderEntry().c_str(), target.c_str(), compileFlags, 0, Dx12Shader->ReleaseAndGetAddress(), errorBlob.GetAddressOf());

    if (errorBlob)
    {
        OutputDebugStringA((char*)errorBlob->GetBufferPointer());
    }

    ThrowIfFailed(hr);

	inShader->SetShaderRHI(std::shared_ptr<RHIShader>(Dx12Shader));
}

RenderResource DX12RHI::CreateCommittedResource(const RHIResourceDesc& inResourceDesc) const
{
    D3D12_RESOURCE_DESC Desc = DX12Helpers::ConvertDesc(inResourceDesc);
    D3D12_HEAP_PROPERTIES bufferHeapType = DX12Helpers::ConvertHeapProperties(inResourceDesc.Flags);

    DX12Resource* Dx12Resource = new DX12Resource();

    ThrowIfFailed(
        D3dDevice->CreateCommittedResource(
            &bufferHeapType
            , D3D12_HEAP_FLAG_NONE
            , &Desc
            , D3D12_RESOURCE_STATE_COMMON
            , nullptr
            , IID_PPV_ARGS(Dx12Resource->GetAddress())
        ));

    return RenderResource(inResourceDesc, Dx12Resource);
}

RenderResource DX12RHI::CreateCommittedResource(const RHIResourceDesc& inResourceDesc, void* InitialData, uint32_t Count, uint32_t Stride) const
{
    RenderResource OutResource = CreateCommittedResource(inResourceDesc);

    if (InitialData)
    {
        RHIResourceDesc UploadBufferDesc = RHIResourceDesc::Buffer(Count, Stride, ResourceFlag::RFLG_Upload, inResourceDesc.Alignment);
        RenderResource UploadBuffer = CreateCommittedResource(UploadBufferDesc);

        D3D12_SUBRESOURCE_DATA subResourceData;
        subResourceData.pData = InitialData;
        subResourceData.RowPitch = Count * Stride;
        subResourceData.SlicePitch = subResourceData.RowPitch;

        std::shared_ptr<DX12Resource> DestResource = OutResource.GetResource<DX12Resource>();
        std::shared_ptr<DX12Resource> SourceResource = UploadBuffer.GetResource<DX12Resource>();

        D3D12_RESOURCE_BARRIER ResourceBarriers[]{
            CD3DX12_RESOURCE_BARRIER::Transition(DestResource->Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST)
            , CD3DX12_RESOURCE_BARRIER::Transition(SourceResource->Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE)
        };
        D3dCmdList->ResourceBarrier(2, ResourceBarriers);

        UpdateSubresources<1>(D3dCmdList.Get(), DestResource->Get(), SourceResource->Get(), 0, 0, 1, &subResourceData);

        D3D12_RESOURCE_BARRIER ResourceBarrier = CD3DX12_RESOURCE_BARRIER::Transition(DestResource->Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
        D3dCmdList->ResourceBarrier(1, &ResourceBarrier);
    }

    return OutResource;
}

void DX12RHI::ExecuteCommandList()
{
    ThrowIfFailed(D3dCmdList->Close());

    ID3D12CommandList* CmdLists[] = { D3dCmdList.Get() };
    D3dCmdQueue->ExecuteCommandLists(_countof(CmdLists), CmdLists);
}

void DX12RHI::FlushCommandQueue()
{
    ++RenderFrameFenceValue;

    ThrowIfFailed(D3dCmdQueue->Signal(RenderFrameFence.Get(), RenderFrameFenceValue));

    if (RenderFrameFence->GetCompletedValue() < RenderFrameFenceValue)
    {
        HANDLE eventHandle = CreateEventEx(nullptr, nullptr, 0, EVENT_ALL_ACCESS);
        ThrowIfFailed(RenderFrameFence->SetEventOnCompletion(RenderFrameFenceValue, eventHandle));

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

void DX12RHI::StartFrame()
{
    ++NumRenderFrame;

    ThrowIfFailed(D3dCmdAlloc->Reset());
    ThrowIfFailed(D3dCmdList->Reset(D3dCmdAlloc.Get(), nullptr));

    D3dCmdList->RSSetViewports(1, &D3dViewport);
    D3dCmdList->RSSetScissorRects(1, &D3dScissorRect);

    D3D12_RESOURCE_BARRIER resourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer().Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));
    D3dCmdList->ResourceBarrier(1, &resourceBarrier);

    D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilHandle = GetDepthStencilHandle();
    D3D12_CPU_DESCRIPTOR_HANDLE BackBufferHandle = GetBackBufferHandle(CurrentBackBufferIndex);

    D3dCmdList->ClearDepthStencilView(DepthStencilHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.f, 0, 0, nullptr);
    D3dCmdList->ClearRenderTargetView(BackBufferHandle, DirectX::Colors::LightSteelBlue, 0, nullptr);
    D3dCmdList->OMSetRenderTargets(1, &BackBufferHandle, true, &DepthStencilHandle);
}

void DX12RHI::EndFrame()
{
    D3D12_RESOURCE_BARRIER resourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(GetCurrentBackBuffer().Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
    D3dCmdList->ResourceBarrier(1, &resourceBarrier);

    ExecuteCommandList();

    ThrowIfFailed(DxgiSwapChain->Present(0, 0));
    CurrentBackBufferIndex = (CurrentBackBufferIndex + 1) % 2;

    FlushCommandQueue();
}