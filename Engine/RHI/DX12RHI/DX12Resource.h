#pragma once
#include <d3d12.h>
#include <d3dx12.h>
#include <wrl/client.h>
#include "../RHIResource.h"
#include "../../Misc/EnumHelpers.h"

namespace DX12Helpers
{
	static inline DXGI_FORMAT ConvertFormat(ResourceFormat inFormat) { return static_cast<DXGI_FORMAT>(inFormat); }

	static inline D3D12_RESOURCE_DIMENSION ConvertResourceType(ResourceType inType)
	{
		if (inType == ResourceType::RT_Buffer) return D3D12_RESOURCE_DIMENSION_BUFFER;
		else if (inType == ResourceType::RT_Texture1D) return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		else if (inType == ResourceType::RT_Texture2D) return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		else if (inType == ResourceType::RT_Texture3D) return D3D12_RESOURCE_DIMENSION_TEXTURE3D;

		return D3D12_RESOURCE_DIMENSION_UNKNOWN;
	}

	static inline D3D12_RESOURCE_FLAGS ConvertResourceFlags(ResourceFlag inFlags)
	{
		D3D12_RESOURCE_FLAGS OutFlags = D3D12_RESOURCE_FLAG_NONE;
		OutFlags |= static_cast<D3D12_RESOURCE_FLAGS>(((inFlags & ResourceFlag::RFLG_RenderTarget) != ResourceFlag::RFLG_None) * D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET);
		OutFlags |= static_cast<D3D12_RESOURCE_FLAGS>(((inFlags & ResourceFlag::RFLG_DepthStencil) != ResourceFlag::RFLG_None) * D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		OutFlags |= static_cast<D3D12_RESOURCE_FLAGS>(((inFlags & ResourceFlag::RFLG_UAV) != ResourceFlag::RFLG_None) * D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS);
		OutFlags |= static_cast<D3D12_RESOURCE_FLAGS>(((inFlags & ResourceFlag::RFLG_NoSRV) != ResourceFlag::RFLG_None) * D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE);
		return OutFlags;
	}

	static inline D3D12_HEAP_PROPERTIES ConvertHeapProperties(ResourceFlag inFlags)
	{
		D3D12_HEAP_TYPE HeapType = D3D12_HEAP_TYPE_DEFAULT;

		if ((inFlags & ResourceFlag::RFLG_Upload) != ResourceFlag::RFLG_None) HeapType = D3D12_HEAP_TYPE_UPLOAD;
		else if ((inFlags & ResourceFlag::RFLG_Readback) != ResourceFlag::RFLG_None) HeapType = D3D12_HEAP_TYPE_READBACK;
		else if ((inFlags & ResourceFlag::RFLG_Custom) != ResourceFlag::RFLG_None) HeapType = D3D12_HEAP_TYPE_CUSTOM;
		else if ((inFlags & ResourceFlag::RFLG_GpuUpload) != ResourceFlag::RFLG_None) HeapType = D3D12_HEAP_TYPE_GPU_UPLOAD;

		return CD3DX12_HEAP_PROPERTIES(HeapType);
	}

	static inline D3D12_RESOURCE_DESC ConvertDesc(const RHIResourceDesc& inDesc)
	{
		D3D12_RESOURCE_DESC OutDesc;
		OutDesc.Dimension = ConvertResourceType(inDesc.Type);
		OutDesc.Alignment = inDesc.Alignment;
		OutDesc.Width = inDesc.Width;
		OutDesc.Height = inDesc.Height;
		OutDesc.DepthOrArraySize = inDesc.Slice;
		OutDesc.MipLevels = inDesc.MipLevels;
		OutDesc.Format = ConvertFormat(inDesc.Format);
		OutDesc.SampleDesc.Count = inDesc.SampleDesc.Count;
		OutDesc.SampleDesc.Quality = inDesc.SampleDesc.Quality;
		OutDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		OutDesc.Flags = ConvertResourceFlags(inDesc.Flags);

		return OutDesc;
	}
}

class DX12Resource : public RHIResource
{
public:
	DX12Resource() = default;

	ID3D12Resource* Get() { return Resource.Get(); }
	ID3D12Resource** GetAddress() { return Resource.GetAddressOf(); }
	ID3D12Resource** ReleaseAndGetAddress() { return Resource.ReleaseAndGetAddressOf(); }
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> Resource;
};