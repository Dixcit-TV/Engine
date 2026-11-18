#pragma once
#include <set>
#include <cassert>
#include "../Misc/Singleton.h"
#include "../RHI/RHIResource.h"
#include "../RHI/RHIInterface.h"

class ShaderManager : public Singleton<ShaderManager>
{
public:
	virtual ~ShaderManager() override = default;
	ShaderManager(const ShaderManager& other) = delete;
	ShaderManager(ShaderManager&& other) = delete;
	ShaderManager& operator=(const ShaderManager& other) = delete;
	ShaderManager& operator=(ShaderManager&& other) = delete;

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
	friend class Singleton<ShaderManager>;

	explicit ShaderManager() = default;

private:
	std::set<ShaderInterface*, ShaderInterface::Cmp> Shaders;
};