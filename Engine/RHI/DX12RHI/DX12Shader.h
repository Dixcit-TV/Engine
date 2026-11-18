#pragma once
#include <wtypes.h>
#include <d3dx12.h>
#include "../RHIResource.h"

class DX12Shader final : public RHIShader
{
public:
	DX12Shader() : RHIShader()
		, CompiledBlob()
	{}

	BYTE* GetBlob() const { return CompiledBlob ? reinterpret_cast<BYTE*>(CompiledBlob->GetBufferPointer()) : nullptr; }
	SIZE_T GetBlobSize() const { return CompiledBlob ? CompiledBlob->GetBufferSize() : 0; }

	ID3DBlob** ReleaseAndGetAddress() { return CompiledBlob.ReleaseAndGetAddressOf(); }

	static LPCSTR TypeToShortTypeName(EShaderType inType)
	{
		switch (inType)
		{
		case VertexShader:
			return "vs";
		case PixelShader:
			return "ps";
		case HullShader:
			return "hs";
		case DomainShader:
			return "ds";
		case GeometryShader:
			return "gs";
		case ComputeShader:
			return "cs";
		default:
			throw;
		}
	}

private:
	Microsoft::WRL::ComPtr<ID3DBlob> CompiledBlob;
};