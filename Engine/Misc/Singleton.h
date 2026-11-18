#pragma once

template<typename Type>
class Singleton
{
public:
	inline static Type& Get() 
	{
		static Type Instance;
		return Instance;
	}

	virtual ~Singleton() = default;
	Singleton(const Singleton& other) = delete;
	Singleton(Singleton&& other) = delete;
	Singleton& operator=(const Singleton& other) = delete;
	Singleton& operator=(Singleton&& other) = delete;

protected:
	explicit Singleton() = default;
};