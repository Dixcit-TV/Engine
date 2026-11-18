#pragma once

struct RHIResourceDesc;
class RenderResource;
class ShaderInterface;

class RHIInterface
{
public:
	virtual ~RHIInterface() = default;

	virtual void FlushCommandQueue() = 0;
	virtual void ExecuteCommandList() = 0;
	virtual void StartFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void CompileShader(ShaderInterface* inShader) const = 0;

	virtual RenderResource CreateCommittedResource(const RHIResourceDesc& inResourceDesc) const = 0;
	virtual RenderResource CreateCommittedResource(const RHIResourceDesc& inResourceDesc, void* InitialData, uint32_t Count, uint32_t Stride) const = 0;
};