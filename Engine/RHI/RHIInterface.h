#pragma once
#include <memory>

struct RHIResourceDesc;
class RenderResource;
class ShaderInterface;
struct Viewport;
class WindowInterface;

enum class RHIType
{
	DX12
};

class RHIInterface
{
public:
	virtual ~RHIInterface() { bHasInstance = false; }

	virtual void FlushCommandQueue() = 0;
	virtual void ExecuteCommandList() = 0;
	virtual void StartFrame() = 0;
	virtual void EndFrame() = 0;

	virtual void CompileShader(ShaderInterface* inShader) const = 0;

	virtual RenderResource* CreateCommittedResource(const RHIResourceDesc& inResourceDesc) const = 0;
	virtual RenderResource* CreateCommittedResource(const RHIResourceDesc& inResourceDesc, RenderResource* inUploadResource, void* InitialData) const = 0;
	virtual RenderResource* CreateCommittedResource(const RHIResourceDesc& inResourceDesc, RenderResource** outUploadResource, void* InitialData) const = 0;

	static std::unique_ptr<RHIInterface> Create(RHIType inType, const Viewport& inViewport, const std::unique_ptr<WindowInterface>& inWindow);

protected:
	explicit RHIInterface() { bHasInstance = true; }

private:
	inline static bool bHasInstance = false;
};