#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Quaternion.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using QuatD = DirectX::XMFLOAT4;

#define CHECK_QUAT(vector, vectorDir)		 \
CHECK(vector.mX == Catch::Approx(vectorDir.x)); \
CHECK(vector.mY == Catch::Approx(vectorDir.y)); \
CHECK(vector.mZ == Catch::Approx(vectorDir.z)); \
CHECK(vector.mW == Catch::Approx(vectorDir.w))

#define CHECK_MATRIX(matrix, matrixDir)		 \
CHECK(matrix[0][0] == Catch::Approx(matrixDir(0, 0))); \
CHECK(matrix[0][1] == Catch::Approx(matrixDir(0, 1))); \
CHECK(matrix[0][2] == Catch::Approx(matrixDir(0, 2))); \
CHECK(matrix[1][0] == Catch::Approx(matrixDir(1, 0))); \
CHECK(matrix[1][1] == Catch::Approx(matrixDir(1, 1))); \
CHECK(matrix[1][2] == Catch::Approx(matrixDir(1, 2))); \
CHECK(matrix[2][0] == Catch::Approx(matrixDir(2, 0))); \
CHECK(matrix[2][1] == Catch::Approx(matrixDir(2, 1))); \
CHECK(matrix[2][2] == Catch::Approx(matrixDir(2, 2)))

XMVECTOR ToQuat(const QuatD& pV4)
{
	return XMLoadFloat4(&pV4);
}

QuatD ToFloatFromQuat(const XMVECTOR& pVector)
{
	QuatD v4F;
	XMStoreFloat4(&v4F, pVector);
	return v4F;
}


TEST_CASE("Quaternion", "[.all][vector][Quaternion]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FQuat empty;
		QuatD emptyD{};
		CHECK_QUAT(empty, emptyD);

		// basic constructor
		FQuat oneParam{ 2.5f };
		QuatD oneParamD{ 2.5f, 2.5f, 2.5f, 2.5f };
		CHECK_QUAT(oneParam, oneParamD);

		FQuat allParam{ 2.5f, .5f, 2.f, 3.f };
		QuatD allParamD{ 2.5f, .5f, 2.f, 3.f };
		CHECK_QUAT(allParam, allParamD);

		// copy constructor
		FQuat copy{ allParam };
		QuatD copyGlm{ allParamD };
		CHECK_QUAT(copy, copyGlm);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_QUAT(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FQuat == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FQuat));
	}

	SECTION("Comparator")
	{
		// compare with self
		FQuat self{ 2.5f, .5f, 2.f, 3.f };
		CHECK(self == self);
		CHECK_FALSE(self != self);

		// compare with same
		CHECK(FQuat{ 2.5f, .5f, 2.f, 3.f } == FQuat{ 2.5f, .5f, 2.f, 3.f });
		CHECK_FALSE(FQuat{ 2.5f, .5f, 2.f, 3.f } != FQuat{ 2.5f, .5f, 2.f, 3.f });

		// trying to find copy/paste mistake
		FQuat twos{ 2.f, 2.f, 2.f, 2.f };
		CHECK_FALSE(FQuat{ .5f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FQuat{ 2.f, .5f, 2.f, 2.f } == twos);
		CHECK_FALSE(FQuat{ 2.f, 2.f, .5f, 2.f } == twos);
		CHECK_FALSE(FQuat{ 2.f, 2.f, 2.f, .5f } == twos);
		CHECK(FQuat{ .5f, 2.f, 2.f, 2.f } != twos);
		CHECK(FQuat{ 2.f, .5f, 2.f, 2.f } != twos);
		CHECK(FQuat{ 2.f, 2.f, .5f, 2.f } != twos);
		CHECK(FQuat{ 2.f, 2.f, 2.f, .5f } != twos);
		CHECK_FALSE(twos == FQuat{ .5f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FQuat{ 2.f, .5f, 2.f, 2.f });
		CHECK_FALSE(twos == FQuat{ 2.f, 2.f, .5f, 2.f });
		CHECK_FALSE(twos == FQuat{ 2.f, 2.f, 2.f, .5f });
		CHECK(twos != FQuat{ .5f, 2.f, 2.f, 2.f });
		CHECK(twos != FQuat{ 2.f, .5f, 2.f, 2.f });
		CHECK(twos != FQuat{ 2.f, 2.f, .5f, 2.f });
		CHECK(twos != FQuat{ 2.f, 2.f, 2.f, .5f });

		// testing comparaision fonctionnality
		FQuat small{ 2.5f, .5f, 2.f, 3.f };
		FQuat big{ 3.75f, 3.f, .75f, 1.5f };

		CHECK_FALSE(self.IsUnit());
		CHECK(FQuat{ 0.5661385171f, 0.1132277034f, 0.4529108137f, 0.6793662205f }.IsUnit());
	}

	SECTION("Constant")
	{
		CHECK_QUAT(FQuat::mOne, QuatD(1.f, 1.f, 1.f, 1.f));

		CHECK_QUAT(FQuat::mZero, QuatD(0.f, 0.f, 0.f, 0.f));
	}

	SECTION("Arithmetic")
	{
		FQuat const small{ 2.5f, .5f, 2.f, 3.f };
		FQuat const big{ 3.75f, 3.f, .75f, 1.5f };

		QuatD const smallD{ 2.5f, .5f, 2.f, 3.f };
		QuatD const bigD{ 3.75f, 3.f, .75f, 1.5f };

		// DirectX doesn't have + and += operand for vec3
		XMVECTOR smallDir = ToQuat(smallD);
		XMVECTOR bigDir = ToQuat(bigD);

		SECTION("Addition")
		{
			QuatD sumD = ToFloatFromQuat((XMVectorAdd(bigDir, smallDir)));

			{
				FQuat sumAssignment = big;
				sumAssignment += small;

				CHECK_QUAT(sumAssignment, sumD);
			}

			{
				FQuat sum = big + small;

				CHECK_QUAT(sum, sumD);
			}
		}

		SECTION("Substraction")
		{
			QuatD sub = ToFloatFromQuat(XMVectorSubtract(bigDir, smallDir));

			{
				FQuat differenceAssignment = big;
				differenceAssignment -= small;

				CHECK_QUAT(differenceAssignment, sub);
			}

			{
				FQuat difference = big - small;

				CHECK_QUAT(difference, sub);
			}
		}

		SECTION("Multiplication")
		{
			/* ---- DirectX Does a pure multiplication ---- */

			/*QuatD mult = ToFloatFromQuat(XMQuaternionMultiply(bigDir, smallDir));
			{
				FQuat productAssignment = big;
				productAssignment *= small;

				CHECK_QUAT(productAssignment, mult);
			}

			{
				FQuat product = big * small;

				CHECK_QUAT(product, mult);
			}*/
		}
	}

	SECTION("Functionality")
	{
		FQuat const base{ 2.5f, .5f, 2.f, 3.f };
		FQuat const other{ 3.75f, 3.f, .75f, 1.5f };

		XMVECTOR const baseD{ 2.5f, .5f, 2.f, 3.f };
		XMVECTOR const otherD{ 3.75f, 3.f, .75f, 1.5f };

		SECTION("Dot")
		{
			float dot = base.DotProduct(other);

			float dotD = ToFloatFromQuat(XMQuaternionDot(baseD, otherD)).x;

			CHECK(dot == dotD);
		}

		SECTION("Magnitude")
		{
			{
				float magnitude = base.Length();

				float magnitudeD = ToFloatFromQuat(XMQuaternionLength(baseD)).x;

				CHECK(magnitude == Catch::Approx(magnitudeD));
			}
		}

		SECTION("Normal")
		{
			FQuat normalize = base;
			normalize.NormalizeSelf();

			QuatD normalizeD = ToFloatFromQuat(XMQuaternionNormalize(baseD));

			CHECK_QUAT(normalize, normalizeD);
		}

		SECTION("Quat/Mat Transition")
		{
			SECTION("To quat")
			{
				FMat3 const baseMat{ 2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f,  2.25f, 1.25f };
				FQuat cQuat = FQuat::GetRotationQuaternion(baseMat);

				XMFLOAT3X3 mat(2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f, 2.25f, 1.25f);
				XMMATRIX baseMatD = XMLoadFloat3x3(&mat);
				XMFLOAT4 dQuat = ToFloatFromQuat(XMQuaternionRotationMatrix(baseMatD));
				CHECK_QUAT(cQuat, dQuat);
			}

			SECTION("To Mat")
			{
				FMat3 cMat = base.GetRotationMatrix();
				XMMATRIX resultMat = XMMatrixRotationQuaternion(baseD);
				XMFLOAT3X3 dMat;
				XMStoreFloat3x3(&dMat, resultMat);

				CHECK_MATRIX(cMat, dMat);
			}
		}

		SECTION("Conjugate")
		{
			CHECK_QUAT(base.Conjugate(), ToFloatFromQuat(XMQuaternionConjugate(baseD)));
		}

		SECTION("Slerp")
		{
			FQuat cSlerp = FQuat::Slerp(base, other, 0.2f);
			XMVECTOR dSlerp = XMQuaternionSlerp(baseD, otherD, 0.2f);

			CHECK_QUAT(cSlerp, ToFloatFromQuat(dSlerp));
		}

		SECTION("Rotation")
		{
			XMFLOAT3 f(1.f, 0.f, 1.f);
			XMVECTOR axis = XMLoadFloat3(&f);
			XMVECTOR dQuat = XMQuaternionRotationAxis(axis, (float)Utilities::DegreesToRadians(56));
			FQuat cQuat = FQuat::RotationAxis((float)Utilities::DegreesToRadians(56), FVec3(1.f, 0.f, 1.f));

			CHECK_QUAT(cQuat, ToFloatFromQuat(dQuat));
		}

		SECTION("Euler")
		{
			{	
				float r = (float)Utilities::DegreesToRadians(32);
				float p = (float)Utilities::DegreesToRadians(88);
				float y = (float)Utilities::DegreesToRadians(45);
				XMVECTOR dResult = XMQuaternionRotationRollPitchYaw(r, p, y);

				FQuat cResult = FQuat::EulerToQuat(r, p, y, FQuat::axis_order::ZXY);

				CHECK_QUAT(cResult, ToFloatFromQuat(dResult));
			}
		}
	}
}