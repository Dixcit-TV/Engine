#pragma once
#include <set>
#include <cassert>
#include "../Misc/Singleton.h"
#include "../RHI/RHIResource.h"
#include "../RHI/RHIInterface.h"

class ShaderCache : public Singleton<ShaderCache>
{
public:
	virtual ~ShaderCache() override = default;
	ShaderCache(const ShaderCache& other) = delete;
	ShaderCache(ShaderCache&& other) = delete;
	ShaderCache& operator=(const ShaderCache& other) = delete;
	ShaderCache& operator=(ShaderCache&& other) = delete;

	void RegisterShader(ShaderInterface* inShader)
	{
		auto Ret = Shaders.insert(inShader);
		assert(Ret.second && "Shader already exists in the ShaderManager!");
	}

	void CompileAllShaders(RHIInterface* RHI) const
	{
		for (ShaderInterface* Shader : Shaders)
		{
			RHI->CompileShader(Shader);
		}
	}

protected:
	friend class Singleton<ShaderCache>;

	explicit ShaderCache() = default;

private:
	std::set<ShaderInterface*, ShaderInterface::Cmp> Shaders;
};