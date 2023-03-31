#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Vec4.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using Dv4 = DirectX::XMFLOAT4;

#define CHECK_VECTOR4(vector, vectorDir)		 \
CHECK(vector.X() == Catch::Approx(vectorDir.x)); \
CHECK(vector.Y() == Catch::Approx(vectorDir.y)); \
CHECK(vector.Z() == Catch::Approx(vectorDir.z)); \
CHECK(vector.W() == Catch::Approx(vectorDir.w))

XMVECTOR ToVector(const Dv4& pV4)
{
	return XMLoadFloat4(&pV4);
}

Dv4 ToFloat(const XMVECTOR& pVector)
{
	Dv4 v4F;
	XMStoreFloat4(&v4F, pVector);
	return v4F;
}

TEST_CASE("Vector4", "[.all][vector][Vector4]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FVec4 empty;
		Dv4 emptyD{};
		CHECK_VECTOR4(empty, emptyD);

		// basic constructor
		FVec4 oneParam{ 2.5f };
		Dv4 oneParamD{ 2.5f, 2.5f, 2.5f, 2.5f };
		CHECK_VECTOR4(oneParam, oneParamD);

		FVec4 allParam{ 2.5f, .5f, 2.f, 3.f };
		Dv4 allParamD{ 2.5f, .5f, 2.f, 3.f };
		CHECK_VECTOR4(allParam, allParamD);

		// copy constructor
		FVec4 copy{ allParam };
		Dv4 copyGlm{ allParamD };
		CHECK_VECTOR4(copy, copyGlm);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_VECTOR4(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FVec4 == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FVec4));
	}

	SECTION("Accessor")
	{
		float const x = 2.5f;
		float const y = .5f;
		float const z = 2.f;
		float const w = 3.f;
		FVec4 vector{ x, y, z, w };

		{
			FVec4 const& vectorConst = vector;
			CHECK(vectorConst[0] == x);
			CHECK(vectorConst[1] == y);
			CHECK(vectorConst[2] == z);
			CHECK(vectorConst[3] == w);
		}

		{
			vector[0] += 1.f;
			vector[1] += 1.f;
			vector[2] += 1.f;
			vector[3] += 1.f;
			CHECK(vector[0] == x + 1.f);
			CHECK(vector[1] == y + 1.f);
			CHECK(vector[2] == z + 1.f);
			CHECK(vector[3] == w + 1.f);
		}
	}

	SECTION("Comparator")
	{
		// compare with self
		FVec4 self{ 2.5f, .5f, 2.f, 3.f };
		CHECK(self == self);
		CHECK_FALSE(self != self);

		// compare with same
		CHECK(FVec4{ 2.5f, .5f, 2.f, 3.f } == FVec4{ 2.5f, .5f, 2.f, 3.f });
		CHECK_FALSE(FVec4{ 2.5f, .5f, 2.f, 3.f } != FVec4{ 2.5f, .5f, 2.f, 3.f });

		// trying to find copy/paste mistake
		FVec4 twos{ 2.f, 2.f, 2.f, 2.f };
		CHECK_FALSE(FVec4{ .5f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FVec4{ 2.f, .5f, 2.f, 2.f } == twos);
		CHECK_FALSE(FVec4{ 2.f, 2.f, .5f, 2.f } == twos);
		CHECK_FALSE(FVec4{ 2.f, 2.f, 2.f, .5f } == twos);
		CHECK(FVec4{ .5f, 2.f, 2.f, 2.f } != twos);
		CHECK(FVec4{ 2.f, .5f, 2.f, 2.f } != twos);
		CHECK(FVec4{ 2.f, 2.f, .5f, 2.f } != twos);
		CHECK(FVec4{ 2.f, 2.f, 2.f, .5f } != twos);
		CHECK_FALSE(twos == FVec4{ .5f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FVec4{ 2.f, .5f, 2.f, 2.f });
		CHECK_FALSE(twos == FVec4{ 2.f, 2.f, .5f, 2.f });
		CHECK_FALSE(twos == FVec4{ 2.f, 2.f, 2.f, .5f });
		CHECK(twos != FVec4{ .5f, 2.f, 2.f, 2.f });
		CHECK(twos != FVec4{ 2.f, .5f, 2.f, 2.f });
		CHECK(twos != FVec4{ 2.f, 2.f, .5f, 2.f });
		CHECK(twos != FVec4{ 2.f, 2.f, 2.f, .5f });

		// testing comparaision fonctionnality
		FVec4 small{ 2.5f, .5f, 2.f, 3.f };
		FVec4 big{ 3.75f, 3.f, .75f, 1.5f };

		CHECK(big.IsLongerThan(small));
		CHECK_FALSE(small.IsLongerThan(big));

		CHECK(small.IsShorterThan(big));
		CHECK_FALSE(big.IsShorterThan(small));

		CHECK_FALSE(self.IsUnit());
		CHECK(FVec4{ 0.5661385171, 0.1132277034, 0.4529108137, 0.6793662205 }.IsUnit());
	}

	SECTION("Constant")
	{
		CHECK_VECTOR4(FVec4::mDown, Dv4(0.f, -1.f, 0.f, 0.f));

		CHECK_VECTOR4(FVec4::mLeft, Dv4(-1.f, 0.f, 0.f, 0.f));

		CHECK_VECTOR4(FVec4::mOne, Dv4(1.f, 1.f, 1.f, 1.f));

		CHECK_VECTOR4(FVec4::mRight, Dv4(1.f, 0.f, 0.f, 0.f));

		CHECK_VECTOR4(FVec4::mUp, Dv4(0.f, 1.f, 0.f, 0.f));

		CHECK_VECTOR4(FVec4::mZero, Dv4(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Arithmetic")
	{
		FVec4 const small{ 2.5f, .5f, 2.f, 3.f };
		FVec4 const big{ 3.75f, 3.f, .75f, 1.5f };

		Dv4 const smallD{ 2.5f, .5f, 2.f, 3.f };
		Dv4 const bigD{ 3.75f, 3.f, .75f, 1.5f };

		// DirectX doesn't have + and += operand for vec3
		XMVECTOR smallDir = ToVector(smallD);
		XMVECTOR bigDir = ToVector(bigD);

		SECTION("Addition")
		{
			Dv4 sumD = ToFloat((XMVectorAdd(bigDir, smallDir)));

			{
				FVec4 sumAssignment = big;
				sumAssignment += small;

				CHECK_VECTOR4(sumAssignment, sumD);
			}

			{
				FVec4 sum = big + small;

				CHECK_VECTOR4(sum, sumD);
			}
		}

		SECTION("Opposite")
		{
			FVec4 opposite = -FVec4{ 3.7f, 3.f, .7f, 1.7f };

			//DirectX doesn't have operator-()
			Dv4 oppositeD = Dv4{ -3.7f, -3.f, -.7f, 1.7f };

			CHECK_VECTOR4(opposite, oppositeD);
		}

		SECTION("Substraction")
		{
			Dv4 sub = ToFloat(XMVectorSubtract(bigDir, smallDir));

			{
				FVec4 differenceAssignment = big;
				differenceAssignment -= small;

				CHECK_VECTOR4(differenceAssignment, sub);
			}

			{
				FVec4 difference = big - small;

				CHECK_VECTOR4(difference, sub);
			}
		}

		SECTION("Multiplication")
		{
			Dv4 mult = ToFloat(XMVectorMultiply(bigDir, smallDir));
			{
				FVec4 productAssignment = big;
				productAssignment *= small;

				CHECK_VECTOR4(productAssignment, mult);
			}

			{
				FVec4 product = big * small;

				CHECK_VECTOR4(product, mult);
			}
		}

		SECTION("Division")
		{
			Dv4 div = ToFloat(XMVectorDivide(bigDir, smallDir));

			{
				FVec4 quotientAssignment = big;
				quotientAssignment /= small;

				CHECK_VECTOR4(quotientAssignment, div);
			}

			{
				FVec4 quotient = big / small;

				CHECK_VECTOR4(quotient, div);
			}
		}
	}

	SECTION("Functionality")
	{
		FVec4 const base{ 2.5f, .5f, 2.f, 3.f };
		FVec4 const other{ 3.75f, 3.f, .75f, 1.5f };

		XMVECTOR const baseD{ 2.5f, .5f, 2.f, 3.f };
		XMVECTOR const otherD{ 3.75f, 3.f, .75f, 1.5f };

		SECTION("Cross")
		{
			FVec4 cross = base.OrthogonalVector();

			Dv4 crossD = ToFloat(XMVector4Orthogonal(baseD));

			CHECK_VECTOR4(cross, crossD);
		}

		SECTION("Dot")
		{
			float dot = base.DotProduct(other);

			float dotD = ToFloat(XMVector4Dot(baseD, otherD)).x;

			CHECK(dot == dotD);
		}

		SECTION("Magnitude")
		{
			{
				float magnitude = base.Length();

				float magnitudeD = ToFloat(XMVector4Length(baseD)).x;

				CHECK(magnitude == Catch::Approx(magnitudeD));
			}
		}

		SECTION("Normal")
		{
			FVec4 normalize = base;
			normalize.NormalizeSelf();

			Dv4 normalizeD = ToFloat(XMVector4Normalize(baseD));

			CHECK_VECTOR4(normalize, normalizeD);
		}

		//SECTION("ProjectOnto")
		//{
		//	FVec4 project = base.ProjectOnTo(other);

		//	Dv4 projectD{ 1.96428f, 1.57142f, 0.39286f, 1.f };

		//	CHECK_VECTOR4(project, projectD);
		//}

		SECTION("ReflectOnto")
		{
			FVec4 reflect = base.Reflexion(other);

			Dv4 reflectD = ToFloat(XMVector4Reflect(baseD, XMVector4Normalize(otherD)));

			CHECK_VECTOR4(reflect, reflectD);
		}
	}
}