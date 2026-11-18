#pragma once
#include <string>

template <size_t N>
struct FixedString
{
	char Value[N];
	constexpr FixedString(const char(&Str)[N])
	{
		std::copy_n(Str, N, Value);
	}
};

template <size_t N>
struct FixedWString
{
	wchar_t Value[N];
	constexpr FixedWString(const wchar_t(&Str)[N])
	{
		std::copy_n(Str, N, Value);
	}
};

