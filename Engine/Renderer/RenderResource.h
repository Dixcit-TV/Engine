#pragma once
#include <wtypes.h>
#include <typeinfo>
#include "../RHI/RHIResource.h"
#include "../Misc/Hash.h"
#include "../Misc/String.h"
#include "ShaderManager.h"

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


template<typename T, FixedWString inFilePath, FixedString inEntry, EShaderType inType>
class Shader : public ShaderInterface
{
public:
	void SetShaderRHI(std::shared_ptr<RHIShader> inShaderRHI) override { ShaderRHI = inShaderRHI; }
	virtual const std::string& GetShaderName() const { return ShaderName; }
	virtual const std::wstring& GetShaderFilePath() const { return FilePath; }
	virtual const std::string& GetShaderEntry() const { return Entry; }
	virtual EShaderType GetShaderType() const { return Type; }

protected:
	explicit Shader() : ShaderInterface()
		, FilePath(inFilePath.Value)
		, ShaderName(typeid(T).name())
		, Entry(inEntry.Value)
		, Hash(0)
		, Type(inType)
	{
		const int TotalLength = FilePath.length() * sizeof(WCHAR) + Entry.length();
		char* TotalString = new char[TotalLength + 1];

		strcpy(TotalString, reinterpret_cast<const char*>(FilePath.c_str()));
		strcat(TotalString, Entry.c_str());

		Hash = Hash::xxh64(TotalString, TotalLength, 0);

		delete[] TotalString;

		ShaderManager::Get().RegisterShader(this);
	}

private:
	std::wstring FilePath;
	std::string ShaderName;
	std::string Entry;
	uint64_t Hash;
	const EShaderType Type;

	std::shared_ptr<RHIShader> ShaderRHI;

	static T Instance;

	uint64_t GetHash() const override { return Hash; }
};


class RenderState
{
private:
	explicit RenderState() = default;
};