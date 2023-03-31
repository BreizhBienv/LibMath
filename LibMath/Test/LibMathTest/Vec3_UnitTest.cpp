#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Vec3.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using Dv3 = DirectX::XMFLOAT3;

#define CHECK_VECTOR3(vector, vectorDir)		 \
CHECK(vector.X() == Catch::Approx(vectorDir.x)); \
CHECK(vector.Y() == Catch::Approx(vectorDir.y)); \
CHECK(vector.Z() == Catch::Approx(vectorDir.z))

XMVECTOR ToVector(const Dv3& pV3)
{
	return XMLoadFloat3(&pV3);
}

Dv3 ToFloat(const XMVECTOR& pVector)
{
	Dv3 v3F;
	XMStoreFloat3(&v3F, pVector);
	return v3F;
}

TEST_CASE("Vector3", "[.all][vector][Vector3]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FVec3 empty;
		Dv3 emptyD{};
		CHECK_VECTOR3(empty, emptyD);

		// basic constructor
		FVec3 oneParam{ 2.5f };
		Dv3 oneParamD{ 2.5f, 2.5f, 2.5f };
		CHECK_VECTOR3(oneParam, oneParamD);

		FVec3 allParam{ 2.5f, .5f, 2.f };
		Dv3 allParamD{ 2.5f, .5f, 2.f };
		CHECK_VECTOR3(allParam, allParamD);

		// copy constructor
		FVec3 copy{ allParam };
		Dv3 copyGlm{ allParamD };
		CHECK_VECTOR3(copy, copyGlm);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_VECTOR3(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FVec3 == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FVec3));
	}

	SECTION("Accessor")
	{
		float const x = 2.5f;
		float const y = .5f;
		float const z = 2.f;
		FVec3 vector{ x, y, z };

		{
			FVec3 const& vectorConst = vector;
			CHECK(vectorConst[0] == x);
			CHECK(vectorConst[1] == y);
			CHECK(vectorConst[2] == z);
		}

		{
			vector[0] += 1.f;
			vector[1] += 1.f;
			vector[2] += 1.f;
			CHECK(vector[0] == x + 1.f);
			CHECK(vector[1] == y + 1.f);
			CHECK(vector[2] == z + 1.f);
		}
	}

	SECTION("Comparator")
	{
		// compare with self
		FVec3 self{ 2.5f, .5f, 2.f };
		CHECK(self == self);
		CHECK_FALSE(self != self);

		// compare with same
		CHECK(FVec3{ 2.5f, .5f, 2.f } == FVec3{ 2.5f, .5f, 2.f });
		CHECK_FALSE(FVec3{ 2.5f, .5f, 2.f } != FVec3{ 2.5f, .5f, 2.f });

		// trying to find copy/paste mistake
		FVec3 twos{ 2.f, 2.f, 2.f };
		CHECK_FALSE(FVec3{ .5f, 2.f, 2.f } == twos);
		CHECK_FALSE(FVec3{ 2.f, .5f, 2.f } == twos);
		CHECK_FALSE(FVec3{ 2.f, 2.f, .5f } == twos);
		CHECK(FVec3{ .5f, 2.f, 2.f } != twos);
		CHECK(FVec3{ 2.f, .5f, 2.f } != twos);
		CHECK(FVec3{ 2.f, 2.f, .5f } != twos);
		CHECK_FALSE(twos == FVec3{ .5f, 2.f, 2.f });
		CHECK_FALSE(twos == FVec3{ 2.f, .5f, 2.f });
		CHECK_FALSE(twos == FVec3{ 2.f, 2.f, .5f });
		CHECK(twos != FVec3{ .5f, 2.f, 2.f });
		CHECK(twos != FVec3{ 2.f, .5f, 2.f });
		CHECK(twos != FVec3{ 2.f, 2.f, .5f });

		// testing comparaision fonctionnality
		FVec3 small{ 2.5f, .5f, 2.f };
		FVec3 big{ 3.75f, 3.f, .75f };

		CHECK(big.IsLongerThan(small));
		CHECK_FALSE(small.IsLongerThan(big));

		CHECK(small.IsShorterThan(big));
		CHECK_FALSE(big.IsShorterThan(small));

		CHECK_FALSE(self.IsUnit());
		CHECK(FVec3{ 0.771516740f, 0.154303357f, 0.617213428f }.IsUnit());
	}

	SECTION("Constant")
	{
		CHECK_VECTOR3(FVec3::mBackward, Dv3(0.f, 0.f, -1.f));

		CHECK_VECTOR3(FVec3::mDown, Dv3(0.f, -1.f, 0.f));

		CHECK_VECTOR3(FVec3::mForward, Dv3(0.f, 0.f, 1.f));

		CHECK_VECTOR3(FVec3::mLeft, Dv3(-1.f, 0.f, 0.f));

		CHECK_VECTOR3(FVec3::mOne, Dv3(1.f, 1.f, 1.f));

		CHECK_VECTOR3(FVec3::mRight, Dv3(1.f, 0.f, 0.f));

		CHECK_VECTOR3(FVec3::mUp, Dv3(0.f, 1.f, 0.f));

		CHECK_VECTOR3(FVec3::mZero, Dv3(0.f, 0.f, 0.f));
	}

	SECTION("Arithmetic")
	{
		FVec3 const small{ 2.5f, .5f, 2.f };
		FVec3 const big{ 3.75f, 3.f, .75f };

		Dv3 const smallD{ 2.5f, .5f, 2.f };
		Dv3 const bigD{ 3.75f, 3.f, .75f };

		// DirectX doesn't have + and += operand for vec3
		XMVECTOR smallDir = ToVector(smallD);
		XMVECTOR bigDir = ToVector(bigD);

		SECTION("Addition")
		{
			Dv3 sumD = ToFloat((XMVectorAdd(bigDir, smallDir)));

			{
				FVec3 sumAssignment = big;
				sumAssignment += small;

				CHECK_VECTOR3(sumAssignment, sumD);
			}

			{
				FVec3 sum = big + small;

				CHECK_VECTOR3(sum, sumD);
			}
		}

		SECTION("Opposite")
		{
			FVec3 opposite = -FVec3{ 3.7f, 3.f, .7f };

			//DirectX doesn't have operator-()
			Dv3 oppositeD = Dv3{ -3.7f, -3.f, -.7f };

			CHECK_VECTOR3(opposite, oppositeD);
		}

		SECTION("Substraction")
		{
			Dv3 sub = ToFloat(XMVectorSubtract(bigDir, smallDir));

			{
				FVec3 differenceAssignment = big;
				differenceAssignment -= small;

				CHECK_VECTOR3(differenceAssignment, sub);
			}

			{
				FVec3 difference = big - small;

				CHECK_VECTOR3(difference, sub);
			}
		}

		SECTION("Multiplication")
		{
			Dv3 mult = ToFloat(XMVectorMultiply(bigDir, smallDir));
			{
				FVec3 productAssignment = big;
				productAssignment *= small;

				CHECK_VECTOR3(productAssignment, mult);
			}

			{
				FVec3 product = big * small;

				CHECK_VECTOR3(product, mult);
			}
		}

		SECTION("Division")
		{
			Dv3 div = ToFloat(XMVectorDivide(bigDir, smallDir));

			{
				FVec3 quotientAssignment = big;
				quotientAssignment /= small;

				CHECK_VECTOR3(quotientAssignment, div);
			}

			{
				FVec3 quotient = big / small;

				CHECK_VECTOR3(quotient, div);
			}
		}
	}
	
	SECTION("Functionality")
	{
		FVec3 const base{ 2.5f, .5f, 2.f };
		FVec3 const other{ 3.75f, 3.f, .75f };

		XMVECTOR const baseD{ 2.5f, .5f, 2.f };
		XMVECTOR const otherD{ 3.75f, 3.f, .75f };

		SECTION("Cross")
		{
			FVec3 cross = base.CrossProduct(other);

			Dv3 crossD = ToFloat(XMVector3Cross(baseD, otherD));

			CHECK_VECTOR3(cross, crossD);
		}

		SECTION("Dot")
		{
			float dot = base.DotProduct(other);

			float dotD = ToFloat(XMVector3Dot(baseD, otherD)).x;

			CHECK(dot == dotD);
		}

		SECTION("Magnitude")
		{
			{
				float magnitude = base.Length();

				float magnitudeD = ToFloat(XMVector3Length(baseD)).x;

				CHECK(magnitude == Catch::Approx(magnitudeD));
			}
		}

		SECTION("Normal")
		{
			FVec3 normalize = base;
			normalize.NormalizeSelf();

			Dv3 normalizeD = ToFloat(XMVector3Normalize(baseD));

			CHECK_VECTOR3(normalize, normalizeD);
		}

		SECTION("ProjectOnto")
		{
			FVec3 project = base.ProjectOnTo(other);

			Dv3 projectD{ 1.96428f, 1.57142f, 0.39286f };

			CHECK_VECTOR3(project, projectD);
		}

		SECTION("ReflectOnto")
		{
			FVec3 reflect = base.Reflexion(other);

			Dv3 reflectD = ToFloat(XMVector3Reflect(baseD, XMVector3Normalize(otherD)));

			CHECK_VECTOR3(reflect, reflectD);
		}
	}
}