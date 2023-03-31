#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Mat3.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using M3 = DirectX::XMFLOAT3X3;

#define CHECK_MATRIX3X3(matrix, matrixDir)		 \
CHECK(matrix[0][0] == Catch::Approx(matrixDir(0, 0))); \
CHECK(matrix[0][1] == Catch::Approx(matrixDir(0, 1))); \
CHECK(matrix[0][2] == Catch::Approx(matrixDir(0, 2))); \
CHECK(matrix[1][0] == Catch::Approx(matrixDir(1, 0))); \
CHECK(matrix[1][1] == Catch::Approx(matrixDir(1, 1))); \
CHECK(matrix[1][2] == Catch::Approx(matrixDir(1, 2))); \
CHECK(matrix[2][0] == Catch::Approx(matrixDir(2, 0))); \
CHECK(matrix[2][1] == Catch::Approx(matrixDir(2, 1))); \
CHECK(matrix[2][2] == Catch::Approx(matrixDir(2, 2)))

XMMATRIX ToMatrix(const M3& pM3)
{
	return XMLoadFloat3x3(&pM3);
}

M3 ToFloat(const XMMATRIX& pMatrix)
{
	M3 m3;
	XMStoreFloat3x3(&m3, pMatrix);
	return m3;
}


TEST_CASE("Matrix3", "[.all][matrix][Matrix3]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FMat3 empty;
		M3 emptyD{};
		CHECK_MATRIX3X3(empty, emptyD);

		// basic constructor
		FMat3 oneParam{ 2.5f };
		M3 oneParamD{ 2.5f, 0.f, 0.f,
					0.f, 2.5f, 0.f,
					0.f, 0.f, 2.5f};
		CHECK_MATRIX3X3(oneParam, oneParamD);

		FMat3 allParam{ FVec3(2.5f, .5f, 2.f),
						FVec3(.5f, 2.f, 2.5f),
						FVec3(2.f, 2.5f, .5f)};
		M3 allParamD{ 2.5f, .5f, 2.f,
					.5f, 2.f, 2.5f,
					2.f, 2.5f, .5f};
		CHECK_MATRIX3X3(allParam, allParamD);

		// copy constructor
		FMat3 copy{ allParam };
		M3 copyD{ allParamD };
		CHECK_MATRIX3X3(copy, copyD);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_MATRIX3X3(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FMat3 == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FMat3));
	}

	SECTION("Accessor")
	{
		FVec3 vector0{ 2.5, .5f, 2.f };
		FVec3 vector1{ 3.5, 1.5f, 3.f };
		FVec3 vector2{ 4.5, 2.5f, 4.f };

		FMat3 mat(vector0, vector1, vector2);

		{
			FMat3 const& MatConst = mat;
			CHECK(MatConst[0] == vector0);
			CHECK(MatConst[1] == vector1);
			CHECK(MatConst[2] == vector2);
		}

		{
			mat[0] += 1.f;
			mat[1] += 1.f;
			mat[2] += 1.f;
			CHECK(mat[0] == vector0 + 1.f);
			CHECK(mat[1] == vector1 + 1.f);
			CHECK(mat[2] == vector2 + 1.f);
		}
	}

	SECTION("Comparator")
	{
		// compare with self
		FVec3 vec0{ 2.5, .5f, 2.f  };
		FVec3 vec1{ 3.5, 1.5f, 3.f };
		FVec3 vec3{ 4.5, 2.5f, 4.f };
		FMat3 Self(vec0, vec1, vec3);

		CHECK(Self == Self);
		CHECK_FALSE(Self != Self);

		// compare with same
		CHECK(FMat3{ 2.5f, .5f, 2.f, 3.5, 1.5f, 3.f, 4.5, 2.5f, 4.f	} ==
			FMat3{ 2.5f, .5f, 2.f, 3.5, 1.5f, 3.f, 4.5, 2.5f, 4.f});
		CHECK_FALSE(FMat3{ 2.5f, .5f, 2.f, 3.5, 1.5f, 3.f, 4.5, 2.5f, 4.f } !=
			FMat3{ 2.5f, .5f, 2.f, 3.5, 1.5f, 3.f, 4.5, 2.5f, 4.f });

		// trying to find copy/paste mistake
		FMat3 twos{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f};
		CHECK_FALSE(FMat3{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f } == twos);
		CHECK_FALSE(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f } == twos);

		CHECK(FMat3{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 5.f, 2.f } != twos);
		CHECK(FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f } != twos);

		CHECK_FALSE(twos == FMat3{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f });
		CHECK_FALSE(twos == FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f });

		CHECK(twos != FMat3{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f });
		CHECK(twos != FMat3{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f });
	}

	SECTION("Constant")
	{
		M3 identity{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f };
		CHECK_MATRIX3X3(FMat3::mIdentity, identity);
	}

	SECTION("Arithmetic")
	{
		FMat3 const small{ 2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f,  2.25f, 1.25f};
		FMat3 const big{ 3.75f, 3.f, .75f, 4.f, 3.25f, 2.75f, 2.25f, 2.5f,  3.f};

		M3 const smallD{ 2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f,  2.25f, 1.25f };
		M3 const bigD{ 3.75f, 3.f, .75f, 4.f, 3.25f, 2.75f, 2.25f, 2.5f,  3.f };

		XMMATRIX smallDir = ToMatrix(smallD);
		XMMATRIX bigDir = ToMatrix(bigD);

		SECTION("Multiplication")
		{
			{
				FMat3 productAssignment = big;
				productAssignment *= small;

				XMMATRIX mult = bigDir;
				mult *= smallDir;

				CHECK_MATRIX3X3(productAssignment, ToFloat(mult));
			}

			{
				FMat3 product = big * small;
				XMMATRIX mult = bigDir * smallDir;

				CHECK_MATRIX3X3(product, ToFloat(mult));
			}
		}
	}

	SECTION("Functionality")
	{
		FMat3 const base{ 2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f,  2.25f, 1.25f };
		FMat3 const other{ 3.75f, 3.f, .75f, 4.f, 3.25f, 2.75f, 2.25f, 2.5f,  3.f };

		M3 const baseD{ 2.5f, .5f, 2.f, 1.5f, .75f, .25f, 2.75f,  2.25f, 1.25f };
		M3 const otherD{ 3.75f, 3.f, .75f, 4.f, 3.25f, 2.75f, 2.25f, 2.5f,  3.f };

		XMMATRIX baseDir = ToMatrix(baseD);
		XMMATRIX otherDir = ToMatrix(otherD);

		SECTION("DotProduct")
		{
			CHECK_MATRIX3X3(base.DotProduct(other), ToFloat(XMMatrixMultiply(baseDir, otherDir)));
		}

		SECTION("Determinant")
		{
			XMFLOAT3 tmp;
			XMStoreFloat3(&tmp, XMMatrixDeterminant(baseDir));
			CHECK(base.Determinant() == tmp.x);
		}

		SECTION("Inverse")
		{
			XMVECTOR vecTmp = XMMatrixDeterminant(baseDir);
			M3 tmp = ToFloat(XMMatrixInverse(&vecTmp, baseDir));
			CHECK_MATRIX3X3(base.Inverse(), tmp);
		}

		SECTION("Identity")
		{
			CHECK_FALSE(base.IsIdentity());
			CHECK_FALSE(XMMatrixIsIdentity(baseDir));

			CHECK(FMat3::mIdentity.IsIdentity());
			CHECK(XMMatrixIsIdentity(ToMatrix(M3{ 1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f })));
		}

		SECTION("Transpose")
		{
			CHECK_MATRIX3X3(base.Transpose(), ToFloat(XMMatrixTranspose(baseDir)));
		}
	}
}