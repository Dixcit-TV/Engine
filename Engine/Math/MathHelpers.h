#pragma once
#include <cstdint>
#include "Quaternion.h"
#include "Vector.h"
#include "Matrix.h"

namespace MathHelpers
{
	inline static constexpr double Pi = 3.14159265358979323846;

	inline uint32_t RoundUpTo(uint32_t value, uint32_t multiple)
	{
		return (value + (multiple - 1)) & ~(multiple - 1);
	}

	inline double DegreesToRadians(double degrees)
	{
		return degrees * (Pi / 180.0);
	}

	inline float DegreesToRadians(float degrees)
	{
		return degrees * (float(Pi) / 180.0f);
	}

	inline double RadiansToDegrees(double radians)
	{
		return radians * (180.0 / Pi);
	}

	inline float RadiansToDegrees(float radians)
	{
		return radians * (180.0f / float(Pi));
	}
};

namespace QuaternionHelpers
{
	template<typename Type>
	inline auto QuatFromEulerRadians(Type inPitch, Type inYaw, Type inRoll)
	{
		const Type HalfPitch = inPitch * 0.5f;
		const Type HalfYaw = inYaw * 0.5f;
		const Type HalfRoll = inRoll * 0.5f;

		const double CY = cos(HalfYaw), SY = sin(HalfYaw);
		const double CP = cos(HalfPitch), SP = sin(HalfPitch);
		const double CR = cos(HalfRoll), SR = sin(HalfRoll);

		Quaternion<Type> Res;
		Res.R = static_cast<Type>(CY * CP * CR + SY * SP * SR);
		Res.I = static_cast<Type>(CY * SP * CR + SY * CP * SR);
		Res.J = static_cast<Type>(SY * CP * CR - CY * SP * SR);
		Res.K = static_cast<Type>(CY * CP * SR - SY * SP * CR);

		return Res;
	}

	template<typename Type>
	inline auto QuatFromEulerDegrees(Type inPitch, Type inYaw, Type inRoll)
	{
		return QuatFromEulerRadians(MathHelpers::DegreesToRadians(inPitch), MathHelpers::DegreesToRadians(inYaw), MathHelpers::DegreesToRadians(inRoll));
	}
};


namespace MatrixHelpers
{
	//Rodriguez formula
	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void Rotate(MatrixType<Type, M, M>& inOutMat, const Quaternion<Type>& inQuat)
	{
		const VectorType Img(inQuat.Img());
		const VectorType Img2x(Img + Img);
		const VectorType<Type, 3> Mat0(inOutMat.M00, inOutMat.M01, inOutMat.M02);
		const VectorType<Type, 3> Mat1(inOutMat.M10, inOutMat.M11, inOutMat.M12);
		const VectorType<Type, 3> Mat2(inOutMat.M20, inOutMat.M21, inOutMat.M22);

		const VectorType Res0(Img2x.Cross(Img.Cross(Mat0) + Mat0 * inQuat.R));
		const VectorType Res1(Img2x.Cross(Img.Cross(Mat1) + Mat1 * inQuat.R));
		const VectorType Res2(Img2x.Cross(Img.Cross(Mat2) + Mat2 * inQuat.R));

		inOutMat.M00 += Res0.X;
		inOutMat.M01 += Res0.Y;
		inOutMat.M02 += Res0.Z;
		inOutMat.M10 += Res1.X;
		inOutMat.M11 += Res1.Y;
		inOutMat.M12 += Res1.Z;
		inOutMat.M20 += Res2.X;
		inOutMat.M21 += Res2.Y;
		inOutMat.M22 += Res2.Z;
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(MatrixType<Type, M, M>& inOutMat, const VectorType<Type, 3>& inEulerAngles)
	{
		Rotate(inOutMat, QuaternionHelpers::QuatFromEulerDegrees(inEulerAngles.X, inEulerAngles.Y, inEulerAngles.Z));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(MatrixType<Type, M, M>& inOutMat, Type inPitch, Type inYaw, Type inRoll)
	{
		Rotate(inOutMat, QuaternionHelpers::QuatFromEulerDegrees(inPitch, inYaw, inRoll));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(MatrixType<Type, M, M>& inOutMat, Type inDegrees, const VectorType<Type, 3>& inUnitAxis)
	{
		Rotate(inOutMat, Quaternion(MathHelpers::DegreesToRadians(inDegrees), inUnitAxis));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(MatrixType<Type, M, M>& inOutMat, const VectorType<Type, 3>& inEulerAngles)
	{
		Rotate(inOutMat, QuaternionHelpers::QuatFromEulerRadians(inEulerAngles.X, inEulerAngles.Y, inEulerAngles.Z));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(MatrixType<Type, M, M>& inOutMat, Type inPitch, Type inYaw, Type inRoll)
	{
		Rotate(inOutMat, QuaternionHelpers::QuatFromEulerRadians(inPitch, inYaw, inRoll));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(MatrixType<Type, M, M>& inOutMat, Type inRadians, const VectorType<Type, 3>& inUnitAxis)
	{
		Rotate(inOutMat, Quaternion(inRadians, inUnitAxis));
	}
};

namespace VectorHelpers
{
	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void Rotate(VectorType<Type, M>& inOutVector, const Quaternion<Type>& inQuat)
	{
		const VectorType Img(inQuat.Img());
		const VectorType Img2x(Img + Img);
		const VectorType<Type, 3> Vec3(inOutVector.X, inOutVector.Y, inOutVector.Z);

		const VectorType<Type, 3> Res = Img2x.Cross(Img.Cross(Vec3) + Vec3 * inQuat.R);
		inOutVector.X += Res.X;
		inOutVector.Y += Res.Y;
		inOutVector.Z += Res.Z;
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(VectorType<Type, M>& inOutVector, const VectorType<Type, 3>& inEulerAngles)
	{
		Rotate(inOutVector, QuaternionHelpers::QuatFromEulerDegrees(inEulerAngles.X, inEulerAngles.Y, inEulerAngles.Z));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(VectorType<Type, M>& inOutVector, Type inPitch, Type inYaw, Type inRoll)
	{
		Rotate(inOutVector, QuaternionHelpers::QuatFromEulerDegrees(inPitch, inYaw, inRoll));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateDegrees(VectorType<Type, M>& inOutVector, Type inDegrees, const VectorType<Type, 3>& inUnitAxis)
	{
		Rotate(inOutVector, Quaternion(MathHelpers::DegreesToRadians(inDegrees), inUnitAxis));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(VectorType<Type, M>& inOutVector, const VectorType<Type, 3>& inEulerAngles)
	{
		Rotate(inOutVector, QuaternionHelpers::QuatFromEulerRadians(inEulerAngles.X, inEulerAngles.Y, inEulerAngles.Z));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(VectorType<Type, M>& inOutVector, Type inPitch, Type inYaw, Type inRoll)
	{
		Rotate(inOutVector, QuaternionHelpers::QuatFromEulerRadians(inPitch, inYaw, inRoll));
	}

	template<typename Type, int M, typename = std::enable_if_t<M == 3 || M == 4>>
	inline void RotateRadians(VectorType<Type, M>& inOutVector, Type inRadians, const VectorType<Type, 3>& inUnitAxis)
	{
		Rotate(inOutVector, Quaternion(inRadians, inUnitAxis));
	}

	template<typename Type>
	inline auto GetDirectionFromEulerRadians(const VectorType<Type, 3>& inEulerAngles)
	{
		VectorType<Type, 3> Dir;
		double CP = cos(inEulerAngles.X);
		Dir.X = static_cast<Type>(CP * sin(inEulerAngles.Y));
		Dir.Y = static_cast<Type>(-sin(inEulerAngles.X));
		Dir.Z = static_cast<Type>(CP * cos(inEulerAngles.Y));

		return Dir;
	}

	template<typename Type>
	inline auto GetDirectionFromEulerDegrees(const VectorType<Type, 3>& inEulerAngles)
	{
		return GetDirectionFromEulerRadians(
			VectorType<Type, 3>(
				MathHelpers::DegreesToRadians(inEulerAngles.X)
				, MathHelpers::DegreesToRadians(inEulerAngles.Y)
				, MathHelpers::DegreesToRadians(inEulerAngles.Z)
			));
	}

	template<typename Type>
	inline auto GetEulerRadiansFrom(const VectorType<Type, 3>& inDirection)
	{
		VectorType<Type, 3> EulerAngles;
		EulerAngles.Y = -atan2f(inDirection.X, inDirection.Z);
		EulerAngles.Z = 0;
		if (EulerAngles.Y < -MathHelpers::Pi || EulerAngles.Y > MathHelpers::Pi)
		{
			EulerAngles.X = asinf(-inDirection.Y);
		}
		else
		{
			EulerAngles.X = asinf(inDirection.Y);
		}

		return EulerAngles;
	}

	template<typename Type>
	inline auto GetEulerDegreesFrom(const VectorType<Type, 3>& inDirection)
	{
		VectorType<Type, 3> EulerAngles = GetEulerRadiansFrom(const VectorType<Type, 3>&inDirection);
		return VectorType<Type, 3>(MathHelpers::RadiansToDegrees(EulerAngles.X), MathHelpers::RadiansToDegrees(EulerAngles.Y), MathHelpers::RadiansToDegrees(EulerAngles.Z));
	}

	template<typename Type, typename OtherType, int M, typename = std::enable_if_t<M == 2 || M == 3>>
	inline auto Direction(const VectorType<Type, M>& First, const VectorType<OtherType, M>& Other)
	{
		VectorType<Type, M> Res(Other - First);
		Res.Normalize();
		return Res;
	}
};