#pragma once
#include "../RHI/RHIResource.h"

class RenderResource
{
public:
	RenderResource(const RHIResourceDesc& inDesc, RHIResource* inResource) : Desc(inDesc), Resource(inResource) {}

	std::shared_ptr<RHIResource> GetResource() { return Resource; }

	template<typename T>
	std::shared_ptr<T> GetResource() { return std::static_pointer_cast<T>(Resource); }
protected:
	RHIResourceDesc Desc;
	std::shared_ptr<RHIResource> Resource;
};

class RenderState
{
private:
	explicit RenderState() = default;
};