#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Vec2.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using Dv2 = DirectX::XMFLOAT2;

#define CHECK_VECTOR2(vector, vectorDir)		 \
CHECK(vector.X() == Catch::Approx(vectorDir.x)); \
CHECK(vector.Y() == Catch::Approx(vectorDir.y))

XMVECTOR ToVector(const Dv2& pV2)
{
	return XMLoadFloat2(&pV2);
}

Dv2 ToFloat(const XMVECTOR& pVector)
{
	Dv2 v2F;
	XMStoreFloat2(&v2F, pVector);
	return v2F;
}

TEST_CASE("Vector2", "[.all][vector][Vector2]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FVec2 empty;
		Dv2 emptyD{};
		CHECK_VECTOR2(empty, emptyD);

		// basic constructor
		FVec2 oneParam{ 2.5f };
		Dv2 oneParamD{ 2.5f, 2.5f };
		CHECK_VECTOR2(oneParam, oneParamD);

		FVec2 allParam{ 2.5f, .5f };
		Dv2 allParamD{ 2.5f, .5f };
		CHECK_VECTOR2(allParam, allParamD);

		// copy constructor
		FVec2 copy{ allParam };
		Dv2 copyGlm{ allParamD };
		CHECK_VECTOR2(copy, copyGlm);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_VECTOR2(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FVec2 == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FVec2));
	}

	SECTION("Accessor")
	{
		float const x = 2.5f;
		float const y = .5f;
		FVec2 vector{ x, y };

		{
			FVec2 const& vectorConst = vector;
			CHECK(vectorConst[0] == x);
			CHECK(vectorConst[1] == y);
		}

		{
			vector[0] += 1.f;
			vector[1] += 1.f;
			CHECK(vector[0] == x + 1.f);
			CHECK(vector[1] == y + 1.f);
		}
	}

	SECTION("Comparator")
	{
		// compare with self
		FVec2 self{ 2.5f, .5f };
		CHECK(self == self);
		CHECK_FALSE(self != self);

		// compare with same
		CHECK(FVec2{ 2.5f, .5f } == FVec2{ 2.5f, .5f });
		CHECK_FALSE(FVec2{ 2.5f, .5f } != FVec2{ 2.5f, .5f });

		// trying to find copy/paste mistake
		FVec2 twos{ 2.f, 2.f };
		CHECK_FALSE(FVec2{ .5f, 2.f } == twos);
		CHECK_FALSE(FVec2{ 2.f, .5f } == twos);
		CHECK_FALSE(FVec2{ 2.f, 0.5f } == twos);
		CHECK(FVec2{ .5f, 2.f } != twos);
		CHECK(FVec2{ 2.f, .5f } != twos);
		CHECK(FVec2{ 2.f, 3.f } != twos);
		CHECK_FALSE(twos == FVec2{ .5f, 2.f });
		CHECK_FALSE(twos == FVec2{ 2.f, .5f });
		CHECK_FALSE(twos == FVec2{ 2.f, 0.5f });
		CHECK(twos != FVec2{ .5f, 2.f });
		CHECK(twos != FVec2{ 2.f, .5f });
		CHECK(twos != FVec2{ 2.f, 3.f });

		// testing comparaision fonctionnality
		FVec2 small{ 2.5f, .5f };
		FVec2 big{ 3.75f, 3.f };

		CHECK(big.IsLongerThan(small));
		CHECK_FALSE(small.IsLongerThan(big));

		CHECK(small.IsShorterThan(big));
		CHECK_FALSE(big.IsShorterThan(small));

		CHECK_FALSE(self.IsUnit());
		CHECK(self.Normalize().IsUnit());
	}

	SECTION("Constant")
	{
		CHECK_VECTOR2(FVec2::mLeft, Dv2(-1.f, 0.f));

		CHECK_VECTOR2(FVec2::mOne, Dv2(1.f, 1.f));

		CHECK_VECTOR2(FVec2::mRight, Dv2(1.f, 0.f));

		CHECK_VECTOR2(FVec2::mUp, Dv2(0.f, 1.f));

		CHECK_VECTOR2(FVec2::mZero, Dv2(0.f, 0.f));
	}

	SECTION("Arithmetic")
	{
		FVec2 const small{ 2.5f, .5f };
		FVec2 const big{ 3.75f, 3.f };

		Dv2 const smallD{ 2.5f, .5f };
		Dv2 const bigD{ 3.75f, 3.f };

		// DirectX doesn't have + and += operand for vec3
		XMVECTOR smallDir = ToVector(smallD);
		XMVECTOR bigDir = ToVector(bigD);

		SECTION("Addition")
		{
			Dv2 sumD = ToFloat((XMVectorAdd(bigDir, smallDir)));

			{
				FVec2 sumAssignment = big;
				sumAssignment += small;

				CHECK_VECTOR2(sumAssignment, sumD);
			}

			{
				FVec2 sum = big + small;

				CHECK_VECTOR2(sum, sumD);
			}
		}

		SECTION("Opposite")
		{
			FVec2 opposite = -FVec2{ 3.7f, 3.f };

			//DirectX doesn't have operator-()
			Dv2 oppositeD = Dv2{ -3.7f, -3.f };

			CHECK_VECTOR2(opposite, oppositeD);
		}

		SECTION("Substraction")
		{
			Dv2 sub = ToFloat(XMVectorSubtract(bigDir, smallDir));

			{
				FVec2 differenceAssignment = big;
				differenceAssignment -= small;

				CHECK_VECTOR2(differenceAssignment, sub);
			}

			{
				FVec2 difference = big - small;

				CHECK_VECTOR2(difference, sub);
			}
		}

		SECTION("Multiplication")
		{
			Dv2 mult = ToFloat(XMVectorMultiply(bigDir, smallDir));
			{
				FVec2 productAssignment = big;
				productAssignment *= small;

				CHECK_VECTOR2(productAssignment, mult);
			}

			{
				FVec2 product = big * small;

				CHECK_VECTOR2(product, mult);
			}
		}

		SECTION("Division")
		{
			Dv2 div = ToFloat(XMVectorDivide(bigDir, smallDir));

			{
				FVec2 quotientAssignment = big;
				quotientAssignment /= small;

				CHECK_VECTOR2(quotientAssignment, div);
			}

			{
				FVec2 quotient = big / small;

				CHECK_VECTOR2(quotient, div);
			}
		}
	}

	SECTION("Functionality")
	{
		FVec2 const base{ 2.5f, .5f };
		FVec2 const other{ 3.75f, 3.f };

		XMVECTOR const baseD{ 2.5f, .5f };
		XMVECTOR const otherD{ 3.75f, 3.f };

		SECTION("Cross")
		{
			FVec2 cross = base.CrossProduct(other);

			Dv2 crossD = ToFloat(XMVector2Cross(baseD, otherD));

			CHECK_VECTOR2(cross, crossD);
		}

		SECTION("Dot")
		{
			float dot = base.DotProduct(other);

			float dotD = ToFloat(XMVector2Dot(baseD, otherD)).x;

			CHECK(dot == dotD);
		}

		SECTION("Magnitude")
		{
			{
				float magnitude = base.Length();

				float magnitudeD = ToFloat(XMVector2Length(baseD)).x;

				CHECK(magnitude == Catch::Approx(magnitudeD));
			}
		}

		SECTION("Normal")
		{
			FVec2 normalize = base;
			normalize.NormalizeSelf();

			Dv2 normalizeD = ToFloat(XMVector2Normalize(baseD));

			CHECK_VECTOR2(normalize, normalizeD);
		}

		SECTION("ProjectOnto")
		{
			FVec2 project = base.ProjectOnTo(other);

			Dv2 projectD{ 1.76829f, 1.41463f };

			CHECK_VECTOR2(project, projectD);
		}

		SECTION("ReflectOnto")
		{
			FVec2 reflect = base.Reflexion(other);

			Dv2 reflectD = ToFloat(XMVector2Reflect(baseD, XMVector2Normalize(otherD)));

			CHECK_VECTOR2(reflect, reflectD);
		}
	}
}