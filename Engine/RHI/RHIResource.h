#pragma once
#include <cstdint>
#include <memory>
#include "../Math/MathHelpers.h"

enum class ResourceFormat : uint8_t
{
	RF_Unknown = 0,
	RF_R32G32B32A32_TYPELESS = 1,
	RF_R32G32B32A32_FLOAT = 2,
	RF_R32G32B32A32_UINT = 3,
	RF_R32G32B32A32_SINT = 4,
	RF_R32G32B32_TYPELESS = 5,
	RF_R32G32B32_FLOAT = 6,
	RF_R32G32B32_UINT = 7,
	RF_R32G32B32_SINT = 8,
	RF_R16G16B16A16_TYPELESS = 9,
	RF_R16G16B16A16_FLOAT = 10,
	RF_R16G16B16A16_UNORM = 11,
	RF_R16G16B16A16_UINT = 12,
	RF_R16G16B16A16_SNORM = 13,
	RF_R16G16B16A16_SINT = 14,
	RF_R32G32_TYPELESS = 15,
	RF_R32G32_FLOAT = 16,
	RF_R32G32_UINT = 17,
	RF_R32G32_SINT = 18,
	RF_R32G8X24_TYPELESS = 19,
	RF_D32_FLOAT_S8X24_UINT = 20,
	RF_R32_FLOAT_X8X24_TYPELESS = 21,
	RF_X32_TYPELESS_G8X24_UINT = 22,
	RF_R10G10B10A2_TYPELESS = 23,
	RF_R10G10B10A2_UNORM = 24,
	RF_R10G10B10A2_UINT = 25,
	RF_R11G11B10_FLOAT = 26,
	RF_R8G8B8A8_TYPELESS = 27,
	RF_R8G8B8A8_UNORM = 28,
	RF_R8G8B8A8_UNORM_SRGB = 29,
	RF_R8G8B8A8_UINT = 30,
	RF_R8G8B8A8_SNORM = 31,
	RF_R8G8B8A8_SINT = 32,
	RF_R16G16_TYPELESS = 33,
	RF_R16G16_FLOAT = 34,
	RF_R16G16_UNORM = 35,
	RF_R16G16_UINT = 36,
	RF_R16G16_SNORM = 37,
	RF_R16G16_SINT = 38,
	RF_R32_TYPELESS = 39,
	RF_D32_FLOAT = 40,
	RF_R32_FLOAT = 41,
	RF_R32_UINT = 42,
	RF_R32_SINT = 43,
	RF_R24G8_TYPELESS = 44,
	RF_D24_UNORM_S8_UINT = 45,
	RF_R24_UNORM_X8_TYPELESS = 46,
	RF_X24_TYPELESS_G8_UINT = 47,
	RF_R8G8_TYPELESS = 48,
	RF_R8G8_UNORM = 49,
	RF_R8G8_UINT = 50,
	RF_R8G8_SNORM = 51,
	RF_R8G8_SINT = 52,
	RF_R16_TYPELESS = 53,
	RF_R16_FLOAT = 54,
	RF_D16_UNORM = 55,
	RF_R16_UNORM = 56,
	RF_R16_UINT = 57,
	RF_R16_SNORM = 58,
	RF_R16_SINT = 59,
	RF_R8_TYPELESS = 60,
	RF_R8_UNORM = 61,
	RF_R8_UINT = 62,
	RF_R8_SNORM = 63,
	RF_R8_SINT = 64,
	RF_A8_UNORM = 65,
	RF_R1_UNORM = 66,
	RF_R9G9B9E5_SHAREDEXP = 67,
	RF_R8G8_B8G8_UNORM = 68,
	RF_G8R8_G8B8_UNORM = 69,
	RF_BC1_TYPELESS = 70,
	RF_BC1_UNORM = 71,
	RF_BC1_UNORM_SRGB = 72,
	RF_BC2_TYPELESS = 73,
	RF_BC2_UNORM = 74,
	RF_BC2_UNORM_SRGB = 75,
	RF_BC3_TYPELESS = 76,
	RF_BC3_UNORM = 77,
	RF_BC3_UNORM_SRGB = 78,
	RF_BC4_TYPELESS = 79,
	RF_BC4_UNORM = 80,
	RF_BC4_SNORM = 81,
	RF_BC5_TYPELESS = 82,
	RF_BC5_UNORM = 83,
	RF_BC5_SNORM = 84,
	RF_B5G6R5_UNORM = 85,
	RF_B5G5R5A1_UNORM = 86,
	RF_B8G8R8A8_UNORM = 87,
	RF_B8G8R8X8_UNORM = 88,
	RF_R10G10B10_XR_BIAS_A2_UNORM = 89,
	RF_B8G8R8A8_TYPELESS = 90,
	RF_B8G8R8A8_UNORM_SRGB = 91,
	RF_B8G8R8X8_TYPELESS = 92,
	RF_B8G8R8X8_UNORM_SRGB = 93,
	RF_BC6H_TYPELESS = 94,
	RF_BC6H_UF16 = 95,
	RF_BC6H_SF16 = 96,
	RF_BC7_TYPELESS = 97,
	RF_BC7_UNORM = 98,
	RF_BC7_UNORM_SRGB = 99,
};

enum class ResourceType : uint8_t
{
	RT_Unknown = 0,
	RT_Buffer = 1,
	RT_Texture1D = 2,
	RT_Texture2D = 3,
	RT_Texture3D = 4
};

enum class ResourceFlag : uint16_t
{
	RFLG_None = 0,
	RFLG_RenderTarget = 1 << 0,
	RFLG_DepthStencil = 1 << 1,
	RFLG_UAV = 1 << 2,
	RFLG_NoSRV = 1 << 3,
	RFLG_Upload = 1 << 4,
	RFLG_Readback = 1 << 5,
	RFLG_Custom = 1 << 6,
	RFLG_GpuUpload = 1 << 7
};

enum class ResourceState : uint32_t
{
	RS_Common = 0,
	RS_ConstantBuffer = 1 << 0,
	RS_IndexBuffer = 1 << 1,
	RS_RenderTarget = 1 << 2,
	RS_UAV = 1 << 3,
	RS_DepthWrite = 1 << 4,
	RS_DepthRead = 1 << 5,
	RS_NonPixelShader = 1 << 6,
	RS_PixelShader = 1 << 7,
	RS_StreamOut = 1 << 8,
	RS_IndirectArg = 1 << 9,
	RS_CopyDest = 1 << 10,
	RS_CopySource = 1 << 11,
	RS_ResolveDest = 1 << 12,
	RS_ResolveSource = 1 << 13,
	RS_RayTracingAccStructure = 1 << 14,
	RS_ShadingRateSource = 1 << 15,
	RS_Present = 1 << 16,
	RS_GenericRead = RS_ConstantBuffer | RS_IndexBuffer | RS_RenderTarget | RS_UAV | RS_IndirectArg | RS_CopySource,
	RS_AllShaders = RS_NonPixelShader | RS_PixelShader
};

struct SampleInfo
{
	uint32_t Count;
	uint32_t Quality;
};

struct RHIResourceDesc
{
	SampleInfo SampleDesc{ 1,0 };
	uint32_t Alignment = 1;
	uint32_t Width = 1;
	uint32_t Height = 1;
	ResourceState States = ResourceState::RS_Common;
	uint16_t Slice = 1;
	ResourceFlag Flags = ResourceFlag::RFLG_None;
	uint8_t MipLevels = 0;
	ResourceType Type = ResourceType::RT_Unknown;
	ResourceFormat Format = ResourceFormat::RF_Unknown;

	static inline RHIResourceDesc Buffer(uint32_t count, uint32_t stride, ResourceFlag inFlags = ResourceFlag::RFLG_None, uint32_t inAlignment = 0)
	{
		return RHIResourceDesc{ { 1,0 }, inAlignment, MathHelpers::RoundUpTo(count * stride, inAlignment), 1, ResourceState::RS_Common, 1, inFlags, 0, ResourceType::RT_Buffer, ResourceFormat::RF_Unknown };
	}

	static inline RHIResourceDesc Texture1D(uint32_t inWidth, ResourceFormat inFormat, uint16_t inSlice = 1, uint8_t inMipLevels = 0, ResourceFlag inFlags = ResourceFlag::RFLG_None, uint32_t inAlignment = 0)
	{
		return RHIResourceDesc{ { 1,0 }, inAlignment, inWidth, 1, ResourceState::RS_Common, inSlice, inFlags, inMipLevels, ResourceType::RT_Texture1D, inFormat };
	}

	static inline RHIResourceDesc Texture2D(uint32_t inWidth, uint32_t inHeight, ResourceFormat inFormat, uint16_t inSlice = 1, uint8_t inMipLevels = 0, ResourceFlag inFlags = ResourceFlag::RFLG_None, uint32_t inAlignment = 0, const SampleInfo& inSampleInfo = { 1,0 })
	{
		return RHIResourceDesc{ inSampleInfo, inAlignment, inWidth, inHeight, ResourceState::RS_Common, inSlice, inFlags, inMipLevels, ResourceType::RT_Texture2D, inFormat };
	}

	static inline RHIResourceDesc Texture3D(uint32_t inWidth, uint32_t inHeight, uint16_t inDepth, ResourceFormat inFormat, uint8_t inMipLevels = 0, ResourceFlag inFlags = ResourceFlag::RFLG_None, uint32_t inAlignment = 0)
	{
		return RHIResourceDesc{ { 1,0 }, inAlignment, inWidth, inHeight, ResourceState::RS_Common, inDepth, inFlags, inMipLevels, ResourceType::RT_Texture3D, inFormat };
	}
};

class RHIResource
{
public:
	virtual ~RHIResource() = default;
protected:
	RHIResource() = default;
};