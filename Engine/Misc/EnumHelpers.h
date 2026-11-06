#pragma once
#include <type_traits>

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator&(T State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return static_cast<Enumtype>(State1) & static_cast<Enumtype>(State2);
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator&=(T& State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return State1 = static_cast<Enumtype>(State1) &= static_cast<Enumtype>(State2);
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator|(T State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return static_cast<T>(static_cast<Enumtype>(State1) | static_cast<Enumtype>(State2));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator|=(T& State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return State1 = static_cast<T>(static_cast<Enumtype>(State1) |= static_cast<Enumtype>(State2));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator^(T State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return static_cast<T>(static_cast<Enumtype>(State1) ^ static_cast<Enumtype>(State2));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator^=(T& State1, T State2)
{
	typedef std::underlying_type<T>::type Enumtype;
	return State1 = static_cast<T>(static_cast<Enumtype>(State1) ^= static_cast<Enumtype>(State2));
}

template<typename T, typename = std::enable_if_t<std::is_enum_v<T>, T>>
constexpr auto operator~(T State)
{
	typedef std::underlying_type<T>::type Enumtype;
	return static_cast<T>(~static_cast<Enumtype>(State));
}