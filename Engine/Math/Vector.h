#pragma once
#include <math.h>
#include <type_traits>
#include <limits>

template<typename Type, int M, typename = std::enable_if_t<std::is_arithmetic_v<Type>, Type>>
struct VectorType;

template<typename Type>
struct VectorType<Type, 2>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type X, Y;
		};
#pragma warning(default:4201)

		Type Data[2];
	};

	static const VectorType UnitX;
	static const VectorType UnitY;
	static const VectorType Zero;
	static const VectorType One;

	inline VectorType(Type Value = 0) : X(Value), Y(Value) {};
	inline VectorType(Type inX, Type inY) : X(inX), Y(inY) {};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(const VectorType<OtherType, 2>& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)) {}
	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(VectorType<OtherType, 2>&& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)) {}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(const VectorType<OtherType, 2>& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(VectorType<OtherType, 2>&& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		return *this;
	}

	template<typename OtherType>
	inline auto Cross(const VectorType<OtherType, 2>& Other) const
	{
		return X * static_cast<Type>(Other.Y) - Y * static_cast<Type>(Other.X);
	}

	template<typename OtherType>
	inline auto Project(const VectorType<OtherType, 2>& Other) const
	{
		return VectorType(Other) * (Dot(*this, Other) / Dot(Other, Other));
	}

	template<typename OtherType>
	inline auto Reject(const VectorType<OtherType, 2>& Other) const
	{
		return *this - Project(Other);
	}

	template<typename OtherType>
	inline auto Reflect(const VectorType<OtherType, 2>& N) const
	{
		return *this - (2 * (Dot(*this, N)) * N);
	}

	inline void Normalize()
	{
		VectorType& ThisVector = *this;
		const double D = Dot(ThisVector, ThisVector);
		if (D < std::numeric_limits<double>::epsilon())
		{
			ThisVector = VectorType::Zero;
			return;
		}

		ThisVector /= sqrt(D);
	}

	inline auto GetNormalized() const
	{
		VectorType Res = *this;
		Res.Normalize();
		return Res;
	}
};

template<typename Type>
const VectorType<Type, 2> VectorType<Type, 2>::UnitX{ Type(1), Type(0) };
template<typename Type>
const VectorType<Type, 2> VectorType<Type, 2>::UnitY{ Type(0), Type(1) };
template<typename Type>
const VectorType<Type, 2> VectorType<Type, 2>::Zero{ };
template<typename Type>
const VectorType<Type, 2> VectorType<Type, 2>::One{ Type(1) };

template<typename Type>
struct VectorType<Type, 3>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type X, Y, Z;
		};
#pragma warning(default:4201)

		Type Data[3];
	};

	static const VectorType UnitX;
	static const VectorType UnitY;
	static const VectorType UnitZ;
	static const VectorType Zero;
	static const VectorType One;

	inline constexpr VectorType(Type Value = 0) : X(Value), Y(Value), Z(Value) {}
	inline constexpr VectorType(Type inX, Type inY, Type inZ) : X(inX), Y(inY), Z(inZ) {}

	template<typename OtherType>
	inline VectorType(const VectorType<OtherType, 2>& inVector, Type inZ = static_cast<Type>(0)) : X(static_cast<Type>(inVector.X)), Y(static_cast<Type>(inVector.Y)), Z(inZ){}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(const VectorType<OtherType, 3>& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)), Z(static_cast<Type>(Other.Z)) {}
	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(VectorType<OtherType, 3>&& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)), Z(static_cast<Type>(Other.Z)) {}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(const VectorType<OtherType, 3>& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		Z = static_cast<Type>(Other.Z);
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(VectorType<OtherType, 3>&& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		Z = static_cast<Type>(Other.Z);
		return *this;
	}

	template<typename OtherType>
	inline auto Cross(const VectorType<OtherType, 3>& Other) const
	{
		return VectorType(
			Y * static_cast<Type>(Other.Z) - Z * static_cast<Type>(Other.Y)
			, -X * static_cast<Type>(Other.Z) + Z * static_cast<Type>(Other.X)
			, X * static_cast<Type>(Other.Y) - Y * static_cast<Type>(Other.X)
		);
	}

	template<typename OtherType>
	inline auto Project(const VectorType<OtherType, 3>& Other) const
	{
		return VectorType(Other) * (Dot(*this, Other) / Dot(Other, Other));
	}

	template<typename OtherType>
	inline auto Reject(const VectorType<OtherType, 3>& Other) const
	{
		return *this - Project(Other);
	}

	template<typename OtherType>
	inline auto Reflect(const VectorType<OtherType, 3>& N) const
	{
		return *this - (2 * (Dot(*this, N)) * N);
	}

	inline void Normalize()
	{
		VectorType& ThisVector = *this;
		const double D = Dot(ThisVector, ThisVector);
		if (D < std::numeric_limits<double>::epsilon())
		{
			ThisVector = VectorType::Zero;
			return;
		}
		
		ThisVector /= sqrt(D);
	}

	inline auto GetNormalized() const
	{
		VectorType Res = *this;
		Res.Normalize();
		return Res;
	}
};

template<typename Type>
const VectorType<Type, 3> VectorType<Type, 3>::UnitX{ Type(1), Type(0), Type(0) };
template<typename Type>
const VectorType<Type, 3> VectorType<Type, 3>::UnitY{ Type(0), Type(1), Type(0) };
template<typename Type>
const VectorType<Type, 3> VectorType<Type, 3>::UnitZ{ Type(0), Type(0), Type(1) };
template<typename Type>
const VectorType<Type, 3> VectorType<Type, 3>::Zero{};
template<typename Type>
const VectorType<Type, 3> VectorType<Type, 3>::One{ Type(1) };

template<typename Type>
struct VectorType<Type, 4>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type X, Y, Z, W;
		};
#pragma warning(default:4201)

		Type Data[4];
	};

	static const VectorType UnitX;
	static const VectorType UnitY;
	static const VectorType UnitZ;
	static const VectorType UnitW;
	static const VectorType Zero;
	static const VectorType One;

	inline VectorType(Type Value = static_cast<Type>(0)) : X(Value), Y(Value), Z(Value), W(Value) {};
	inline VectorType(Type inX, Type inY, Type inZ, Type inW = Type(0)) : X(inX), Y(inY), Z(inZ), W(inW) {};

	template<typename OtherType>
	inline VectorType(const VectorType<OtherType, 2>& inVector, Type inZ = static_cast<Type>(0), Type inW = static_cast<Type>(0)) : X(static_cast<Type>(inVector.X)), Y(static_cast<Type>(inVector.Y)), Z(inZ), W(inW) {}
	template<typename OtherType>
	inline VectorType(const VectorType<OtherType, 3>& inVector, Type inW = static_cast<Type>(0)) : X(static_cast<Type>(inVector.X)), Y(static_cast<Type>(inVector.Y)), Z(static_cast<Type>(inVector.Z)), W(inW) {}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(const VectorType<OtherType, 4>& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)), Z(static_cast<Type>(Other.Z)), W(static_cast<Type>(Other.W)) {}
	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline VectorType(VectorType<OtherType,4>&& Other) : X(static_cast<Type>(Other.X)), Y(static_cast<Type>(Other.Y)), Z(static_cast<Type>(Other.Z)) , W(static_cast<Type>(Other.W)) {}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(const VectorType<OtherType,4>& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		Z = static_cast<Type>(Other.Z);
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline auto& operator=(VectorType<OtherType,4>&& Other)
	{
		X = static_cast<Type>(Other.X);
		Y = static_cast<Type>(Other.Y);
		Z = static_cast<Type>(Other.Z);
		return *this;
	}
};

template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::UnitX{ Type(1), Type(0), Type(0), Type(0)	};
template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::UnitY{ Type(0), Type(1), Type(0), Type(0)	};
template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::UnitZ{ Type(0), Type(0), Type(1), Type(0)	};
template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::UnitW{ Type(0), Type(0), Type(0), Type(1)	};
template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::Zero{};
template<typename Type>
const VectorType<Type, 4> VectorType<Type, 4>::One{ Type(1) };

template<typename Type, typename OtherType, int M>
inline auto& operator*=(VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] *= static_cast<Type>(Other.Data[I]);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator*=(VectorType<Type, M>& This, OtherType Scalar)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] *= static_cast<Type>(Scalar);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator/=(VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] /= static_cast<Type>(Other.Data[I]);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator/=(VectorType<Type, M>& This, OtherType Scalar)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] /= static_cast<Type>(Scalar);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator+=(VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] += static_cast<Type>(Other.Data[I]);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator+=(VectorType<Type, M>& This, OtherType Scalar)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] += static_cast<Type>(Scalar);
	}
	return This;
}


template<typename Type, typename OtherType, int M>
inline auto& operator-=(VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] -= static_cast<Type>(Other.Data[I]);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto& operator-=(VectorType<Type, M>& This, OtherType Scalar)
{
	for (int I = 0; I < M; ++I)
	{
		This.Data[I] -= static_cast<Type>(Scalar);
	}
	return This;
}

template<typename Type, typename OtherType, int M>
inline auto operator*(const VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	VectorType<Type, M> Res = This;
	return Res *= Other;
}

template<typename Type, typename OtherType, int M>
inline auto operator*(const VectorType<Type, M>& This, OtherType Scalar)
{
	VectorType<Type, M> Res = This;
	return Res *= Scalar;
}

template<typename Type, typename OtherType, int M>
inline auto operator/(const VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	VectorType<Type, M> Res = This;
	return Res /= Other;
}

template<typename Type, typename OtherType, int M>
inline auto operator/(const VectorType<Type, M>& This, OtherType Scalar)
{
	VectorType<Type, M> Res = This;
	return Res /= Scalar;
}

template<typename Type, typename OtherType, int M>
inline auto operator+(const VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	VectorType<Type, M> Res = This;
	return Res += Other;
}

template<typename Type, typename OtherType, int M>
inline auto operator+(const VectorType<Type, M>& This, OtherType Scalar)
{
	VectorType<Type, M> Res = This;
	return Res += Scalar;
}

template<typename Type, typename OtherType, int M>
inline auto operator-(const VectorType<Type, M>& This, const VectorType<OtherType, M>& Other)
{
	VectorType<Type, M> Res = This;
	return Res -= Other;
}

template<typename Type, typename OtherType, int M>
inline auto operator-(const VectorType<Type, M>& This, OtherType Scalar)
{
	VectorType<Type, M> Res = This;
	return Res -= Scalar;
}

template<typename Type, int M>
inline auto operator-(const VectorType<Type, M>& This)
{
	VectorType<Type, M> Res = This;
	return Res *= -1;
}

template<typename Type, int M>
inline bool operator==(const VectorType<Type, M>& This, const VectorType<Type, M>& Other)
{
	constexpr Type Epsilon = std::numeric_limits<Type>::epsilon;

	for (int I = 0; I < M; ++I)
	{
		if (abs(This.Data[I] - Other.Data[I]) > Epsilon)
		{
			return false;
		}
	}
	return true;
}

template<typename Type, int M>
inline bool operator!=(const VectorType<Type, M>& This, const VectorType<Type, M>& Other)
{
	return !(This == Other);
}

template<typename Type, typename OtherType, int M>
inline Type Dot(const VectorType<Type, M>& First, const VectorType<OtherType, M>& Other)
{
	Type DotRes = Type(0);
	for (int I = 0; I < M; ++I)
	{
		DotRes += First.Data[I] * static_cast<Type>(Other.Data[I]);
	}

	return DotRes;
}

template<typename Type, int M>
inline double Length(const VectorType<Type, M>& This)
{
	return sqrt(Dot(This, This));
}

using Vector2F = VectorType<float, 2>;
using Vector2D = VectorType<double, 2>;
using Vector2B = VectorType<char, 2>;
using Vector2UB = VectorType<unsigned char, 2>;
using Vector2S = VectorType<short, 2>;
using Vector2US = VectorType<unsigned short, 2>;
using Vector2I = VectorType<int, 2>;
using Vector2UI = VectorType<unsigned int, 2>;
using Vector2LL = VectorType<long long, 2>;
using Vector2ULL = VectorType<unsigned long long, 2>;

using Vector3F = VectorType<float, 3>;
using Vector3D = VectorType<double, 3>;
using Vector3B = VectorType<char, 3>;
using Vector3UB = VectorType<unsigned char, 3>;
using Vector3S = VectorType<short, 3>;
using Vector3US = VectorType<unsigned short, 3>;
using Vector3I = VectorType<int, 3>;
using Vector3UI = VectorType<unsigned int, 3>;
using Vector3LL = VectorType<long long, 3>;
using Vector3ULL = VectorType<unsigned long long, 3>;

using Vector4F = VectorType<float, 4>;
using Vector4D = VectorType<double, 4>;
using Vector4B = VectorType<char, 4>;
using Vector4UB = VectorType<unsigned char, 4>;
using Vector4S = VectorType<short, 4>;
using Vector4US = VectorType<unsigned short, 4>;
using Vector4I = VectorType<int, 4>;
using Vector4UI = VectorType<unsigned int, 4>;
using Vector4LL = VectorType<long long, 4>;
using Vector4ULL = VectorType<unsigned long long, 4>;

using Vector2 = Vector2F;
using Vector3 = Vector3F;
using Vector4 = Vector4F;