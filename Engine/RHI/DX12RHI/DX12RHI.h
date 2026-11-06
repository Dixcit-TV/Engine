#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_3.h>
#include <wrl/client.h>
#include <exception>

#include "../RHIInterface.h"

struct Viewport;
struct RHIResourceDesc;
class WindowInterface;
class RenderResource;

using namespace Microsoft::WRL;

class DX12RHI final : public RHIInterface
{
public:
	DX12RHI(const Viewport& inViewport, const WindowInterface* const inWindow);
	~DX12RHI() override;

	void FlushCommandQueue() override;
	void ExecuteCommandList() override;
	void StartFrame() override;
	void EndFrame() override;

	RenderResource CreateCommittedResource(const RHIResourceDesc& inResourceDesc);
	RenderResource CreateCommittedResource(const RHIResourceDesc& inResourceDesc, void* InitialData, uint32_t Count, uint32_t Stride);

private:
	void ThrowIfFailed(HRESULT hr) const
	{
		if (FAILED(hr))
		{
			// Set a breakpoint on this line to catch Win32 API errors.
			throw ComException(hr);
		}
	}

	inline ComPtr<ID3D12Resource> GetCurrentBackBuffer() { return BackBuffers[CurrentBackBufferIndex]; }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferHandle(INT Index) { return CD3DX12_CPU_DESCRIPTOR_HANDLE(RtvHeapDescriptor->GetCPUDescriptorHandleForHeapStart(), Index, RtvDescriptorSize); }
	inline D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilHandle() { return DsvHeapDescriptor->GetCPUDescriptorHandleForHeapStart(); }

private:
	class ComException : public std::exception
	{
	public:
		ComException(HRESULT hr) : result(hr) {}

		const char* what() const noexcept override
		{
			static char s_str[64] = {};
			sprintf_s(s_str, "Failure with HRESULT of %08X",
				static_cast<unsigned int>(result));
			return s_str;
		}

	private:
		HRESULT result;
	};

	D3D12_VIEWPORT D3dViewport;
	D3D12_RECT D3dScissorRect;

	ComPtr<ID3D12Device> D3dDevice;
	ComPtr<IDXGIFactory> DxgiFactory;
	ComPtr<IDXGISwapChain> DxgiSwapChain;

	ComPtr<ID3D12Resource> BackBuffers[2];
	ComPtr<ID3D12Resource> DepthStencilResource;

	ComPtr<ID3D12CommandQueue> D3dCmdQueue;
	ComPtr<ID3D12CommandAllocator> D3dCmdAlloc;
	ComPtr<ID3D12GraphicsCommandList> D3dCmdList;

	ComPtr<ID3D12DescriptorHeap> RtvHeapDescriptor;
	ComPtr<ID3D12DescriptorHeap> DsvHeapDescriptor;

	ComPtr<ID3D12Fence> RenderFrameFence;
	UINT64 RenderFrameFenceValue = 0;

	UINT64 NumRenderFrame = 0;

	UINT RtvDescriptorSize = 0;
	UINT DsvDescriptorSize = 0;
	UINT CbvSrvDescriptorSize = 0;

	UINT CurrentBackBufferIndex = 0;

	UINT Msaa4xQuality = 0;
	bool MsaaEnabled = false;
};

