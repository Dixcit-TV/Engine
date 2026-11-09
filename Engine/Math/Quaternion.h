#pragma once
#include "Vector.h"
#include "Matrix.h"

template<typename Type>
struct Quaternion
{
	union {
#pragma warning(disable:4201)
		struct {
			Type R, I, J, K;
		};
#pragma warning(default:4201)

		Type Data[4];
	};

	inline Quaternion(Type inReal = Type(1), Type inI = Type(0), Type inJ = Type(0), Type inK = Type(0)) : R(inReal), I(inI), J(inJ), K(inK) {}
	inline Quaternion(double inRadAngle, const VectorType<Type, 3>& inAxis)
	{  
		double halfAngle = inRadAngle / 2.f;
		double s = sin(halfAngle);
		R = static_cast<Type>(cos(halfAngle));
		I = static_cast<Type>(inAxis.X * s);
		J = static_cast<Type>(inAxis.Y * s);
		K = static_cast<Type>(inAxis.Z * s);
	}
	inline Quaternion(Type inI, Type inJ, Type inK) : R(0), I(inI), J(inJ), K(inK) {  }

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline Quaternion(const Quaternion<OtherType>& Other) : R(static_cast<Type>(Other.R)), I(static_cast<Type>(Other.I)), J(static_cast<Type>(Other.J)), K(static_cast<Type>(Other.K)) {}
	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline Quaternion(Quaternion<OtherType>&& Other) : R(static_cast<Type>(Other.R)), I(static_cast<Type>(Other.I)), J(static_cast<Type>(Other.J)), K(static_cast<Type>(Other.K)) {}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline Quaternion& operator =(const Quaternion<OtherType>& Other)
	{
		R = static_cast<Type>(Other.R);
		I = static_cast<Type>(Other.I);
		J = static_cast<Type>(Other.J);
		K = static_cast<Type>(Other.K);
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline Quaternion& operator =(Quaternion<OtherType>&& Other)
	{
		R = static_cast<Type>(Other.R);
		I = static_cast<Type>(Other.I);
		J = static_cast<Type>(Other.J);
		K = static_cast<Type>(Other.K);
		return *this;
	}

	inline Type Real() const { return R; };
	inline VectorType<Type, 3> Img() const { return VectorType<Type, 3>(I, J, K); };

	template<typename OtherType>
	inline Quaternion& operator *=(OtherType scalar)
	{
		R *= static_cast<Type>(scalar);
		I *= static_cast<Type>(scalar);
		J *= static_cast<Type>(scalar);
		K *= static_cast<Type>(scalar);
		return *this;
	}

	template<typename OtherType>
	inline Quaternion& operator /=(OtherType scalar)
	{  
		R /= static_cast<Type>(scalar);
		I /= static_cast<Type>(scalar);
		J /= static_cast<Type>(scalar);
		K /= static_cast<Type>(scalar);
		return *this;
	}

	template<typename OtherType>
	inline Quaternion operator *(const Quaternion<OtherType> Other) const
	{
		return Quaternion<Type>{ R * static_cast<Type>(Other.R) - I * static_cast<Type>(Other.I) - J * static_cast<Type>(Other.J) - K * static_cast<Type>(Other.K)
							, R * static_cast<Type>(Other.I) + I * static_cast<Type>(Other.R) + J * static_cast<Type>(Other.K) - K * static_cast<Type>(Other.J)
							, R * static_cast<Type>(Other.J) + J * static_cast<Type>(Other.R) - I * static_cast<Type>(Other.K) + K * static_cast<Type>(Other.I)
							, R * static_cast<Type>(Other.K) + K * static_cast<Type>(Other.R) + I * static_cast<Type>(Other.J) - J * static_cast<Type>(Other.I)};
	}
	template<typename OtherType>
	inline Quaternion operator *(OtherType scalar) const { return Quaternion<Type>{ *this } *= scalar; }
	template<typename OtherType>
	inline Quaternion operator /(OtherType scalar) const { return Quaternion<Type>{ *this } /= scalar; }

	inline Type MagnitudeSquared() const { return R * R + I * I + J * J + K * K;  }
	inline Type Magnitude() const { return static_cast<Type>(sqrt(MagnitudeSquared())); }
	inline Quaternion<Type> Inverse() const { return Conjugate() /= MagnitudeSquared(); }
	inline Quaternion<Type> Conjugate() const { return Quaternion<Type>{ R, -I, -J, -K }; }

	inline void Normalize() { (*this) /= Magnitude(); }
};

using QuatF = Quaternion<float>;
using QuatD = Quaternion<double>;

using Quat = QuatF;
