#pragma once
#include <type_traits>
#include "Vector.h"

template<typename Type, int M, int N, typename = std::enable_if_t<std::is_arithmetic_v<Type>, Type>>
struct MatrixType
{
	Type Data[N][M];

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(const MatrixType<OtherType, M, N>& Other)
	{
		for (int R = 0; R < N; ++R)
		{
			for (int C = 0; C < M; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(MatrixType<OtherType, M, N>&& Other)
	{
		for (int C = 0; C < M; ++C)
		{
			for (int R = 0; R < N; ++R)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(const MatrixType<OtherType, M, N>& Other)
	{
		for (int R = 0; R < N; ++R)
		{
			for (int C = 0; C < M; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(MatrixType<OtherType, M, N>&& Other)
	{
		for (int C = 0; C < M; ++C)
		{
			for (int R = 0; R < N; ++R)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	static inline MatrixType<Type, M, M> Identity()
	{
		MatrixType<Type, M, M> Res;
		memset(*Res, 0, sizeof(Res));

		for (int I = 0; I < M; ++I)
		{
			Res.Data[I][I] = Type(1);
		}

		return Res;
	}

	// Gaussian Elimination determinant calculation for arbitrary matrix size
	template<typename = std::enable_if_t<M == N>>
	inline double Determinant() const
	{
		double Det = 0.0;
		constexpr Type Epsilon = std::numeric_limits<Type>::epsilon();

		for (int I = 0; I < M; ++I)
		{
			int Pivot = I;
			for (int J = I + 1; J < M; ++J)
			{
				if (abs(Data[J][I]) > abs(Data[Pivot][I]))
				{
					Pivot = J;
				}
			}

			if (abs(Data[Pivot][I]) < Epsilon)
			{
				return 0.0;
			}

			if (Pivot != I)
			{
				std::swap(Data[I], Data[Pivot]);
				Det = -Det;
			}

			Det *= Data[I][I];

			for (int J = I + 1; J < M; ++J)
			{
				Type Factor = Data[J][I] / Data[I][J];
				for (int K = I; K < M; ++K)
				{
					Data[J][K] -= Factor * Data[I][K];
				}
			}
		}

		return Det;
	}

	// Gaussian Elimination Inverse calculation for arbitrary matrix size
	template<typename = std::enable_if_t<M == N>>
	inline auto Inverse() const
	{
		MatrixType Inverse = Identity();
		constexpr Type Epsilon = std::numeric_limits<Type>::epsilon();

		for (int I = 0; I < M; ++I)
		{
			int Pivot = I;
			Type MaxVal = abs(Data[I][I]);
			for (int J = I + 1; J < M; ++J)
			{
				if (abs(Data[J][I]) > MaxVal)
				{
					MaxVal = abs(Data[J][I]);
					Pivot = J;
				}
			}

			if (MaxVal < Epsilon)
			{
				return IdentityMatrix<Type, M, M>();
			}

			if (Pivot != I)
			{
				std::swap(Data[I], Data[Pivot]);
				std::swap(Inverse.Data[I], Inverse.Data[Pivot]);
			}

			Type Scale = 1 / Data[I][I];
			for (int J = 0; J < M; ++J)
			{
				Data[I][J] *= Scale;
				Inverse.Data[I][J] *= Scale;
			}

			for (int J = 0; J < M; ++J)
			{
				if (J != I)
				{
					Type Factor = Data[J][I];
					for (int K = I; K < M; ++K)
					{
						Data[J][K] -= Factor * Data[I][K];
						Inverse.Data[J][K] -= Factor * Inverse.Data[I][K];
					}
				}
			}
		}

		return Inverse;
	}
};

template<typename Type>
struct MatrixType<Type, 2, 2>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type M00, M01
				, M10, M11;
		};
#pragma warning(default:4201)

		Type Data[2][2];
	};

	static const MatrixType Identity;
	static const MatrixType One;
	static const MatrixType Zero;

	inline MatrixType(Type Value = Type(0))
		: M00(Value), M01(Value)
		, M10(Value), M11(Value)
	{
	};

	inline MatrixType(Type inM00, Type inM01, Type inM10, Type inM11)
		: M00(inM00), M01(inM01)
		, M10(inM10), M11(inM11)
	{
	};

	inline MatrixType(const VectorType<Type, 2>& inV1, const VectorType<Type, 2>& inV2)
		: M00(inV1.X), M01(inV1.Y)
		, M10(inV2.X), M11(inV2.Y)
	{
	};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(const MatrixType<OtherType, 2, 2>& Other)
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(MatrixType<OtherType, 2, 2>&& Other)
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(const MatrixType<OtherType, 2, 2>& Other)
	{
		for (int R = 0; R < 2; ++R)
		{
			for (int C = 0; C < 2; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(MatrixType<OtherType, 2, 2>&& Other)
	{
		for (int R = 0; R < 2; ++R)
		{
			for (int C = 0; C < 2; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	inline double Determinant() const
	{
		return double(M00 * M11) - double(M01 * M10);
	}

	inline auto Inverse() const
	{
		MatrixType Inverse = Identity;
		const double Det = Determinant();
		if (Det < std::numeric_limits<double>::epsilon())
		{
			return Inverse;
		}

		const double InvDet = 1.0 / Det;

		Inverse.M00 = Type(M11 * InvDet);
		Inverse.M01 = Type(-M01 * InvDet);

		Inverse.M10 = Type(-M10 * InvDet);
		Inverse.M11 = Type(M00 * InvDet);

		return Inverse;
	}
};

template<typename Type>
const MatrixType<Type, 2, 2> MatrixType<Type, 2, 2>::Identity{ VectorType<Type, 2>::UnitX, VectorType<Type, 2>::UnitY };
template<typename Type>
const MatrixType<Type, 2, 2> MatrixType<Type, 2, 2>::One{ Type(1) };
template<typename Type>
const MatrixType<Type, 2, 2> MatrixType<Type, 2, 2>::Zero{};

template<typename Type>
struct MatrixType<Type, 3, 3>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type M00, M01, M02
				, M10, M11, M12
				, M20, M21, M22;
		};
#pragma warning(default:4201)

		Type Data[3][3];
	};

	static const MatrixType Identity;
	static const MatrixType One;
	static const MatrixType Zero;

	inline MatrixType(Type Value = Type(0))
		: M00(Value), M01(Value), M02(Value)
		, M10(Value), M11(Value), M12(Value)
		, M20(Value), M21(Value), M22(Value)
	{};

	inline MatrixType(Type inM00, Type inM01, Type inM02, Type inM10, Type inM11, Type inM12, Type inM20, Type inM21, Type inM22)
		: M00(inM00), M01(inM01), M02(inM02)
		, M10(inM10), M11(inM11), M12(inM12)
		, M20(inM20), M21(inM21), M22(inM22)
	{};

	inline MatrixType(const VectorType<Type, 3>& inV1, const VectorType<Type, 3>& inV2, const VectorType<Type, 3>& inV3)
		: M00(inV1.X), M01(inV1.Y), M02(inV1.Z)
		, M10(inV2.X), M11(inV2.Y), M12(inV2.Z)
		, M20(inV3.X), M21(inV3.Y), M22(inV3.Z)
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(const MatrixType<OtherType, 3, 3>& Other)
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01)), M02(static_cast<Type>(Other.M02))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11)), M12(static_cast<Type>(Other.M12))
		, M20(static_cast<Type>(Other.M20)), M21(static_cast<Type>(Other.M21)), M22(static_cast<Type>(Other.M22))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(MatrixType<OtherType, 3, 3>&& Other)
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01)), M02(static_cast<Type>(Other.M02))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11)), M12(static_cast<Type>(Other.M12))
		, M20(static_cast<Type>(Other.M20)), M21(static_cast<Type>(Other.M21)), M22(static_cast<Type>(Other.M22))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(const MatrixType<OtherType, 3, 3>& Other)
	{
		for (int R = 0; R < 3; ++R)
		{
			for (int C = 0; C < 3; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(MatrixType<OtherType, 3, 3>&& Other)
	{
		for (int R = 0; R < 3; ++R)
		{
			for (int C = 0; C < 3; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	inline double Determinant() const
	{
		const double C1 = double(M00) * (double(M11 * M22) - double(M12 * M21));
		const double C2 = double(M01) * (double(M10 * M22) - double(M12 * M20));
		const double C3 = double(M02) * (double(M10 * M21) - double(M11 * M20));
		return C1 - C2 + C3;
	}

	inline auto Inverse() const
	{
		MatrixType Inverse = Identity;
		const double Det = Determinant();
		if (Det < std::numeric_limits<double>::epsilon())
		{
			return Inverse;
		}

		const double InvDet = 1.0 / Det;

		Inverse.M00 = static_cast<Type>((M11 * M22 - M21 * M12) * InvDet);
		Inverse.M01 = static_cast<Type>((M02 * M21 - M01 * M22) * InvDet);
		Inverse.M02 = static_cast<Type>((M01 * M12 - M02 * M11) * InvDet);

		Inverse.M10 = static_cast<Type>((M12 * M20 - M10 * M22) * InvDet);
		Inverse.M11 = static_cast<Type>((M00 * M22 - M02 * M20) * InvDet);
		Inverse.M12 = static_cast<Type>((M02 * M10 - M00 * M12) * InvDet);

		Inverse.M20 = static_cast<Type>((M10 * M21 - M11 * M20) * InvDet);
		Inverse.M21 = static_cast<Type>((M01 * M20 - M00 * M21) * InvDet);
		Inverse.M22 = static_cast<Type>((M00 * M11 - M01 * M10) * InvDet);

		return Inverse;
	}
};

template<typename Type>
const MatrixType<Type, 3, 3> MatrixType<Type, 3, 3>::Identity{ VectorType<Type, 3>::UnitX, VectorType<Type, 3>::UnitY, VectorType<Type, 3>::UnitZ };
template<typename Type>
const MatrixType<Type, 3, 3> MatrixType<Type, 3, 3>::One{ Type(1) };
template<typename Type>
const MatrixType<Type, 3, 3> MatrixType<Type, 3, 3>::Zero{};

template<typename Type>
struct MatrixType<Type, 4, 4>
{
	union {
#pragma warning(disable:4201)
		struct {
			Type M00, M01, M02, M03
				, M10, M11, M12, M13
				, M20, M21, M22, M23
				, M30, M31, M32, M33;
		};
#pragma warning(default:4201)

		Type Data[4][4];
	};

	static const MatrixType Identity;
	static const MatrixType One;
	static const MatrixType Zero;

	inline MatrixType(Type Value = 0)
		: M00(Value), M01(Value), M02(Value), M03(Value)
		, M10(Value), M11(Value), M12(Value), M13(Value)
		, M20(Value), M21(Value), M22(Value), M23(Value)
		, M30(Value), M31(Value), M32(Value), M33(Value)
	{
	};

	inline MatrixType(Type inM00, Type inM01, Type inM02, Type inM03, Type inM10, Type inM11, Type inM12, Type inM13, Type inM20, Type inM21, Type inM22, Type inM23, Type inM30, Type inM31, Type inM32, Type inM33)
		: M00(inM00), M01(inM01), M02(inM02), M03(inM03)
		, M10(inM10), M11(inM11), M12(inM12), M13(inM13)
		, M20(inM20), M21(inM21), M22(inM22), M23(inM23)
		, M30(inM30), M31(inM31), M32(inM32), M33(inM33)
	{
	};

	inline MatrixType(const VectorType<Type, 4>& inV1, const VectorType<Type, 4>& inV2, const VectorType<Type, 4>& inV3, const VectorType<Type, 4>& inV4)
		: M00(inV1.X), M01(inV1.Y), M02(inV1.Z), M03(inV1.W)
		, M10(inV2.X), M11(inV2.Y), M12(inV2.Z), M13(inV2.W)
		, M20(inV3.X), M21(inV3.Y), M22(inV3.Z), M23(inV3.W)
		, M30(inV4.X), M31(inV4.Y), M32(inV4.Z), M33(inV4.W)
	{
	};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(const MatrixType<OtherType, 4, 4>& Other) 
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01)), M02(static_cast<Type>(Other.M02)), M03(static_cast<Type>(Other.M03))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11)), M12(static_cast<Type>(Other.M12)), M13(static_cast<Type>(Other.M13))
		, M20(static_cast<Type>(Other.M20)), M21(static_cast<Type>(Other.M21)), M22(static_cast<Type>(Other.M22)), M23(static_cast<Type>(Other.M23))
		, M30(static_cast<Type>(Other.M30)), M31(static_cast<Type>(Other.M31)), M32(static_cast<Type>(Other.M32)), M33(static_cast<Type>(Other.M33))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType(MatrixType<OtherType, 4, 4>&& Other)
		: M00(static_cast<Type>(Other.M00)), M01(static_cast<Type>(Other.M01)), M02(static_cast<Type>(Other.M02)), M03(static_cast<Type>(Other.M03))
		, M10(static_cast<Type>(Other.M10)), M11(static_cast<Type>(Other.M11)), M12(static_cast<Type>(Other.M12)), M13(static_cast<Type>(Other.M13))
		, M20(static_cast<Type>(Other.M20)), M21(static_cast<Type>(Other.M21)), M22(static_cast<Type>(Other.M22)), M23(static_cast<Type>(Other.M23))
		, M30(static_cast<Type>(Other.M30)), M31(static_cast<Type>(Other.M31)), M32(static_cast<Type>(Other.M32)), M33(static_cast<Type>(Other.M33))
	{};

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(const MatrixType<OtherType, 4, 4>& Other)
	{
		for (int R = 0; R < 4; ++R)
		{
			for (int C = 0; C < 4; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	template<typename OtherType, typename = std::enable_if_t<!std::is_same_v<Type, OtherType>>>
	inline MatrixType& operator=(MatrixType<OtherType, 4, 4>&& Other)
	{
		for (int R = 0; R < 4; ++R)
		{
			for (int C = 0; C < 4; ++C)
			{
				Data[R][C] = static_cast<Type>(Other.Data[R][C]);
			}
		}
		return *this;
	}

	inline double Determinant() const
	{
		double Res = 0.0;

		const double X2233 = double(M22 * M33);
		const double X2133 = double(M21 * M33);
		const double X2132 = double(M21 * M32);
		const double X2232 = double(M23 * M32);
		const double X2331 = double(M23 * M31);
		const double X2231 = double(M22 * M31);
		double SubDet = double(M11) * (X2233 - X2232) - double(M12) * (X2133 - X2331) + double(M13) * (X2132 - X2231);
		Res += double(M00) * SubDet;

		const double X2033 = double(M20 * M33);
		const double X2032 = double(M20 * M32);
		const double X2330 = double(M23 * M30);
		const double X2230 = double(M22 * M30);
		SubDet = double(M10) * (X2233 - X2232) - double(M12) * (X2033 - X2330) + double(M13) * (X2032 - X2230);
		Res -= double(M01) * SubDet;

		const double X2031 = double(M20 * M31);
		const double X2130 = double(M21 * M30);
		SubDet = double(M10) * (X2133 - X2331) - double(M11) * (X2033 - X2330) + double(M13) * (X2031 - X2130);
		Res += double(M02) * (SubDet);

		SubDet = double(M10) * (X2132 - X2231) - double(M11) * (X2032 - X2230) + double(M12) * (X2031 - X2130);
		Res -= double(M03) * (SubDet);

		return Res;
	}

	inline auto Inverse() const
	{
		MatrixType Inverse = Identity;

		Inverse.M00 = M11 * M22 * M33 - M11 * M23 * M32 - M21 * M12 * M33 + M21 * M13 * M32 + M31 * M12 * M23 - M31 * M13 * M22;
		Inverse.M01 = -M01 * M22 * M33 + M01 * M23 * M32 + M21 * M02 * M33 - M21 * M03 * M32 - M31 * M02 * M23 + M31 * M03 * M22;
		Inverse.M02 = M01 * M12 * M33 - M01 * M13 * M32 - M11 * M02 * M33 + M11 * M03 * M32 + M31 * M02 * M13 - M31 * M03 * M12;
		Inverse.M03 = -M01 * M12 * M23 + M01 * M13 * M22 + M11 * M02 * M23 - M11 * M03 * M22 - M21 * M02 * M13 + M21 * M03 * M12;

		Inverse.M10 = -M10 * M22 * M33 + M10 * M23 * M32 + M20 * M12 * M33 - M20 * M13 * M32 - M30 * M12 * M23 + M30 * M13 * M22;
		Inverse.M11 = M00 * M22 * M33 - M00 * M23 * M32 - M20 * M02 * M33 + M20 * M03 * M32 + M30 * M02 * M23 - M30 * M03 * M22;
		Inverse.M12 = -M00 * M12 * M33 + M00 * M13 * M32 + M10 * M02 * M33 - M10 * M03 * M32 - M30 * M02 * M13 + M30 * M03 * M12;
		Inverse.M13 = M00 * M12 * M23 - M00 * M13 * M22 - M10 * M02 * M23 + M10 * M03 * M22 + M20 * M02 * M13 - M20 * M03 * M12;

		Inverse.M20 = M10 * M21 * M33 - M10 * M23 * M31 - M20 * M11 * M33 + M20 * M13 * M31 + M30 * M11 * M23 - M30 * M13 * M21;
		Inverse.M21 = -M00 * M21 * M33 + M00 * M23 * M31 + M20 * M01 * M33 - M20 * M03 * M31 - M30 * M01 * M23 + M30 * M03 * M21;
		Inverse.M22 = M00 * M11 * M33 - M00 * M13 * M31 - M10 * M01 * M33 + M10 * M03 * M31 + M30 * M01 * M13 - M30 * M03 * M11;
		Inverse.M23 = -M00 * M11 * M23 + M00 * M13 * M21 + M10 * M01 * M23 - M10 * M03 * M21 - M20 * M01 * M13 + M20 * M03 * M11;

		Inverse.M30 = -M10 * M21 * M32 + M10 * M22 * M31 + M20 * M11 * M32 - M20 * M12 * M31 - M30 * M11 * M22 + M30 * M12 * M21;
		Inverse.M31 = M00 * M21 * M32 - M00 * M22 * M31 - M20 * M01 * M32 + M20 * M02 * M31 + M30 * M01 * M22 - M30 * M02 * M21;
		Inverse.M32 = -M00 * M11 * M32 + M00 * M12 * M31 + M10 * M01 * M32 - M10 * M02 * M31 - M30 * M01 * M12 + M30 * M02 * M11;
		Inverse.M33 = M00 * M11 * M22 - M00 * M12 * M21 - M10 * M01 * M22 + M10 * M02 * M21 + M20 * M01 * M12 - M20 * M02 * M11;

		const double Det = double(M00) * Inverse.M00 + double(M01) * Inverse.M10 + double(M02) * Inverse.M20 + double(M03) * Inverse.M30;
		if (abs(Det) < std::numeric_limits<double>::epsilon())
		{
			return MatrixType<Type, 4, 4>::Indentity;
		}

		const double InvDet = 1.0 / Det;
		for (int R = 0; R < 4; ++R)
		{
			for (int C = 0; C < 4; ++C)
			{
				Inverse.MRC *= InvDet;
			}
		}

		return Inverse;
	}
};

template<typename Type>
const MatrixType<Type, 4, 4> MatrixType<Type, 4, 4>::Identity{ VectorType<Type, 4>::UnitX, VectorType<Type, 4>::UnitY, VectorType<Type, 4>::UnitZ, VectorType<Type, 4>::UnitW };
template<typename Type>
const MatrixType<Type, 4, 4> MatrixType<Type, 4, 4>::One{ Type(1) };
template<typename Type>
const MatrixType<Type, 4, 4> MatrixType<Type, 4, 4>::Zero{};

template<typename Type, typename OtherType, int M, int N>
inline auto& operator*=(MatrixType<Type, M, N>& This, const MatrixType<OtherType, M, M>& Other)
{
	Type Sums[M];
	for (int R = 0; R < N; ++R)
	{
		memset(Sums, 0, sizeof(Sums));
		for (int C2 = 0; C2 < M; ++C2)
		{
			for (int C = 0; C < M; ++C)
			{
				Sums[C2] += static_cast<Type>(This.Data[R][C]) * static_cast<Type>(Other.Data[C][C2]);
			}
		}
		memcpy(This.Data[R], Sums, sizeof(Sums));
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator*=(MatrixType<Type, M, N>& This, OtherType Scalar)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] *= static_cast<Type>(Scalar);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator/=(MatrixType<Type, M, N>& This, OtherType Scalar)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] /= static_cast<Type>(Scalar);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator+=(MatrixType<Type, M, N>& This, const MatrixType<OtherType, M, N>& Other)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] += static_cast<Type>(Other.Data[R][C]);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator+=(MatrixType<Type, M, N>& This, OtherType Scalar)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] += static_cast<Type>(Scalar);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator-=(MatrixType<Type, M, N>& This, const MatrixType<OtherType, M, N>& Other)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] -= static_cast<Type>(Other.Data[R][C]);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N>
inline auto& operator-=(MatrixType<Type, M, N>& This, OtherType Scalar)
{
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			This.Data[R][C] -= static_cast<Type>(Scalar);
		}
	}
	return This;
}

template<typename Type, typename OtherType, int M, int N, int O>
inline auto operator*(const MatrixType<Type, M, N>& First, const MatrixType<OtherType, O, M>& Other)
{
	MatrixType<Type, O, M> RetValue(First);
	return RetValue *= Other;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator*(const MatrixType<Type, M, N>& First, OtherType Scalar)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue *= Scalar;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator/(const MatrixType<Type, M, N>& First, OtherType Scalar)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue /= Scalar;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator+(const MatrixType<Type, M, N>& First, const MatrixType<OtherType, M, N>& Other)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue += Other;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator+(const MatrixType<Type, M, N>& First, OtherType Scalar)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue += Scalar;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator-(const MatrixType<Type, M, N>& First, const MatrixType<OtherType, M, N>& Other)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue -= Other;
}

template<typename Type, typename OtherType, int M, int N>
inline auto operator-(const MatrixType<Type, M, N>& First, OtherType Scalar)
{
	MatrixType<Type, M, N> RetValue(First);
	return RetValue -= Scalar;
}

template<typename Type, int M, int N>
inline bool operator==(const MatrixType<Type, M, N>& First, const MatrixType<Type, M, N>& Other)
{
	Type Epsilon = std::numeric_limits<Type>::epsilon();
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			if (abs(First.Data[R][C] - Other.Data[R][C]) > Epsilon)
			{
				return false;
			}
		}
	}

	return true;
}

template<typename Type, int M, int N>
inline bool operator!=(const MatrixType<Type, M, N>& First, const MatrixType<Type, M, N>& Other)
{
	return !(First == Other);
}

template<typename Type, int M, int N>
inline auto Transpose(const MatrixType<Type, M, N>& Matrix)
{
	MatrixType<Type, N, M> RetValue;
	for (int R = 0; R < N; ++R)
	{
		for (int C = 0; C < M; ++C)
		{
			RetValue.Data[C][R] = Matrix.Data[R][C];
		}
	}

	return RetValue;
}

using Matrix22F = MatrixType<float, 2, 2>;
using Matrix22D = MatrixType<double, 2, 2>;
using Matrix22B = MatrixType<char, 2, 2>;
using Matrix22UB = MatrixType<unsigned char, 2, 2>;
using Matrix22S = MatrixType<short, 2, 2>;
using Matrix22US = MatrixType<unsigned short, 2, 2>;
using Matrix22I = MatrixType<int, 2, 2>;
using Matrix22UI = MatrixType<unsigned int, 2, 2>;
using Matrix22LL = MatrixType<long long, 2, 2>;
using Matrix22ULL = MatrixType<unsigned long long, 2, 2>;

using Matrix33F = MatrixType<float, 3, 3>;
using Matrix33D = MatrixType<double, 3, 3>;
using Matrix33B = MatrixType<char, 3, 3>;
using Matrix33UB = MatrixType<unsigned char, 3, 3>;
using Matrix33S = MatrixType<short, 3, 3>;
using Matrix33US = MatrixType<unsigned short, 3, 3>;
using Matrix33I = MatrixType<int, 3, 3>;
using Matrix33UI = MatrixType<unsigned int, 3, 3>;
using Matrix33LL = MatrixType<long long, 3, 3>;
using Matrix33ULL = MatrixType<unsigned long long, 3, 3>;

using Matrix44F = MatrixType<float, 4, 4>;
using Matrix44D = MatrixType<double, 4, 4>;
using Matrix44B = MatrixType<char, 4, 4>;
using Matrix44UB = MatrixType<unsigned char, 4, 4>;
using Matrix44S = MatrixType<short, 4, 4>;
using Matrix44US = MatrixType<unsigned short, 4, 4>;
using Matrix44I = MatrixType<int, 4, 4>;
using Matrix44UI = MatrixType<unsigned int, 4, 4>;
using Matrix44LL = MatrixType<long long, 4, 4>;
using Matrix44ULL = MatrixType<unsigned long long, 4, 4>;

using Matrix22 = Matrix22F;
using Matrix33 = Matrix33F;
using Matrix44 = Matrix44F;