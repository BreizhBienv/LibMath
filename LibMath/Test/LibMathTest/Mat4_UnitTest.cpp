#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include "../../LibMath/Mat4.h"
#include <DirectXMath.h>

using namespace lm;
using namespace DirectX;
using M4 = DirectX::XMFLOAT4X4;

#define CHECK_MATRIX4X4(matrix, matrixDir)		 \
CHECK(matrix[0][0] == Catch::Approx(matrixDir(0, 0))); \
CHECK(matrix[0][1] == Catch::Approx(matrixDir(0, 1))); \
CHECK(matrix[0][2] == Catch::Approx(matrixDir(0, 2))); \
CHECK(matrix[0][3] == Catch::Approx(matrixDir(0, 3))); \
CHECK(matrix[1][0] == Catch::Approx(matrixDir(1, 0))); \
CHECK(matrix[1][1] == Catch::Approx(matrixDir(1, 1))); \
CHECK(matrix[1][2] == Catch::Approx(matrixDir(1, 2))); \
CHECK(matrix[1][3] == Catch::Approx(matrixDir(1, 3))); \
CHECK(matrix[2][0] == Catch::Approx(matrixDir(2, 0))); \
CHECK(matrix[2][1] == Catch::Approx(matrixDir(2, 1))); \
CHECK(matrix[2][2] == Catch::Approx(matrixDir(2, 2))); \
CHECK(matrix[2][3] == Catch::Approx(matrixDir(2, 3))); \
CHECK(matrix[3][0] == Catch::Approx(matrixDir(3, 0))); \
CHECK(matrix[3][1] == Catch::Approx(matrixDir(3, 1))); \
CHECK(matrix[3][2] == Catch::Approx(matrixDir(3, 2))); \
CHECK(matrix[3][3] == Catch::Approx(matrixDir(3, 3)))

XMMATRIX ToMatrix(const M4& pM4)
{
	return XMLoadFloat4x4(&pM4);
}

M4 ToFloat(const XMMATRIX& pMatrix)
{
	M4 M4;
	XMStoreFloat4x4(&M4, pMatrix);
	return M4;
}


TEST_CASE("Matrix4", "[.all][matrix][Matrix4]")
{
	SECTION("Instantiation")
	{
		// default constructor
		FMat4 empty;
		M4 emptyD{};
		CHECK_MATRIX4X4(empty, emptyD);

		// basic constructor
		FMat4 oneParam{ 2.5f };
		M4 oneParamD{ 2.5f, 0.f, 0.f, 0.f,
					0.f, 2.5f, 0.f, 0.f,
					0.f, 0.f, 2.5f, 0.f,
					0.f, 0.f, 0.f, 2.5f};
		CHECK_MATRIX4X4(oneParam, oneParamD);

		FMat4 allParam{ FVec4(2.5f, .5f, 2.f, 1.f),
						FVec4(.5f, 2.f, 2.5f, 3.f),
						FVec4(2.f, 2.5f, .5f, 1.5f),
						FVec4(0.5f, 1.75f, 2.f, 1.f)};
		M4 allParamD{ 2.5f, .5f, 2.f, 1.f,
					.5f, 2.f, 2.5f, 3.f,
					2.f, 2.5f, .5f, 1.5f,
					0.5f, 1.75f, 2.f, 1.f};
		CHECK_MATRIX4X4(allParam, allParamD);

		// copy constructor
		FMat4 copy{ allParam };
		M4 copyD{ allParamD };
		CHECK_MATRIX4X4(copy, copyD);

		// assignment operator
		empty = allParam;
		emptyD = allParamD;
		CHECK_MATRIX4X4(empty, emptyD);

		// DirectX compatibility
		//CHECK(sizeof FMat4 == sizeof XMVECTOR);
		//CHECK(memcmp(&allParam, &allParamD, sizeof FMat4));
	}

	SECTION("Accessor")
	{
		FVec4 vector0{ 2.5f, .5f, 2.f, 0.25f};
		FVec4 vector1{ 3.5f, 1.5f, 3.f, 2.f};
		FVec4 vector2{ 4.5f, 2.5f, 4.f, 3.f };
		FVec4 vector3{ 3.f, 2.5f, 4.f, 3.5 };

		FMat4 mat(vector0, vector1, vector2, vector3);

		{
			FMat4 const& MatConst = mat;
			CHECK(MatConst[0] == vector0);
			CHECK(MatConst[1] == vector1);
			CHECK(MatConst[2] == vector2);
			CHECK(MatConst[3] == vector3);
		}

		{
			mat[0] += 1.f;
			mat[1] += 1.f;
			mat[2] += 1.f;
			mat[3] += 1.f;
			CHECK(mat[0] == vector0 + 1.f);
			CHECK(mat[1] == vector1 + 1.f);
			CHECK(mat[2] == vector2 + 1.f);
			CHECK(mat[3] == vector3 + 1.f);
		}
	}

	SECTION("Comparator")
	{
		// compare with self
		FVec4 vec0{ 2.5f, .5f, 2.f, 3.f };
		FVec4 vec1{ 3.5f, 1.5f, 3.f, 2.f };
		FVec4 vec2{ 4.5f, 2.5f, 4.f, 3.f };
		FVec4 vec3{ 3.5f, 3.f, 4.f,  2.f};
		FMat4 Self(vec0, vec1, vec2, vec3);

		CHECK(Self == Self);
		CHECK_FALSE(Self != Self);

		// compare with same
		CHECK(FMat4{ 2.5f, .5f, 2.f, 3.f,
			3.5, 1.5f, 3.f, 2.f,
			4.5, 2.5f, 4.f, 3.f,
			3.5f, 3.f, 4.f, 2.f } ==
			FMat4{ 2.5f, .5f, 2.f, 3.f,
			3.5, 1.5f, 3.f, 2.f, 
			4.5, 2.5f, 4.f, 3.f,
			 3.5f, 3.f, 4.f,  2.f });
		CHECK_FALSE(FMat4{ 2.5f, .5f, 2.f, 3.f,
			3.5, 1.5f, 3.f, 2.f,
			4.5, 2.5f, 4.f, 3.f,
			 3.5f, 3.f, 4.f,  2.f } !=
			FMat4{ 2.5f, .5f, 2.f, 3.f,
			3.5, 1.5f, 3.f, 2.f,
			4.5, 2.5f, 4.f, 3.f,
			 3.5f, 3.f, 4.f,  2.f });

		// trying to find copy/paste mistake
		FMat4 twos{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f };
		CHECK_FALSE(FMat4{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f } == twos);
		CHECK_FALSE(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f } == twos);

		CHECK(FMat4{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 5.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f } != twos);
		CHECK(FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f } != twos);

		CHECK_FALSE(twos == FMat4{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f });
		CHECK_FALSE(twos == FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f });

		CHECK(twos != FMat4{ .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f, 2.f });
		CHECK(twos != FMat4{ 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, 2.f, .5f });
	}

	SECTION("Constant")
	{
		M4 identity{ 1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f};
		CHECK_MATRIX4X4(FMat4::mIdentity, identity);
	}

	SECTION("Arithmetic")
	{
		FMat4 const small{ 2.5f, .5f, 2.f, 1.5f,
			1.5f, .75f, .25f, 1.f,
			2.75f,  2.25f, 1.25f, 2.f,
			2.25f, 1.75f, 0.5f, 3.f};
		FMat4 const big{ 3.75f, 3.f, .75f, 2.f,
			4.f, 3.25f, 2.75f, 2.5f,
			2.25f, 2.5f,  3.f, 3.5f,
			3.f, 2.5f, 4.f, 3.75f};

		M4 const smallD{ 2.5f, .5f, 2.f, 1.5f,
			1.5f, .75f, .25f, 1.f,
			2.75f,  2.25f, 1.25f, 2.f,
			2.25f, 1.75f, 0.5f, 3.f };
		M4 const bigD{ 3.75f, 3.f, .75f, 2.f,
			4.f, 3.25f, 2.75f, 2.5f,
			2.25f, 2.5f,  3.f, 3.5f,
			3.f, 2.5f, 4.f, 3.75f };

		XMMATRIX smallDir = ToMatrix(smallD);
		XMMATRIX bigDir = ToMatrix(bigD);

		SECTION("Multiplication")
		{
			{
				FMat4 productAssignment = big;
				productAssignment *= small;

				XMMATRIX mult = bigDir;
				mult *= smallDir;

				CHECK_MATRIX4X4(productAssignment, ToFloat(mult));
			}

			{
				FMat4 product = big * small;
				XMMATRIX mult = bigDir * smallDir;

				CHECK_MATRIX4X4(product, ToFloat(mult));
			}
		}
	}

	SECTION("Functionality")
	{
		FMat4 const base{ 2.5f, .5f, 2.f, 1.5f,
			1.5f, .75f, .25f, 1.f,
			2.75f,  2.25f, 1.25f, 2.f,
			2.25f, 1.75f, 0.5f, 3.f };
		FMat4 const other{ 3.75f, 3.f, .75f, 2.f,
			4.f, 3.25f, 2.75f, 2.5f,
			2.25f, 2.5f,  3.f, 3.5f,
			3.f, 2.5f, 4.f, 3.75f };

		M4 const baseD{ 2.5f, .5f, 2.f, 1.5f,
			1.5f, .75f, .25f, 1.f,
			2.75f,  2.25f, 1.25f, 2.f,
			2.25f, 1.75f, 0.5f, 3.f };
		M4 const otherD{ 3.75f, 3.f, .75f, 2.f,
			4.f, 3.25f, 2.75f, 2.5f,
			2.25f, 2.5f,  3.f, 3.5f,
			3.f, 2.5f, 4.f, 3.75f };

		XMMATRIX baseDir = ToMatrix(baseD);
		XMMATRIX otherDir = ToMatrix(otherD);

		SECTION("DotProduct")
		{
			CHECK_MATRIX4X4(base.DotProduct(other), ToFloat(XMMatrixMultiply(baseDir, otherDir)));
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
			M4 tmp = ToFloat(XMMatrixInverse(&vecTmp, baseDir));
			CHECK_MATRIX4X4(base.Inverse(), tmp);
		}

		SECTION("Identity")
		{
			CHECK_FALSE(base.IsIdentity());
			CHECK_FALSE(XMMatrixIsIdentity(baseDir));

			CHECK(FMat4::mIdentity.IsIdentity());
			CHECK(XMMatrixIsIdentity(ToMatrix(M4{ 1.f, 0.f, 0.f, 0.f,
				0.f, 1.f, 0.f, 0.f,
				0.f, 0.f, 1.f, 0.f,
				0.f, 0.f, 0.f, 1.f})));
		}

		SECTION("Transpose")
		{
			CHECK_MATRIX4X4(base.Transpose(), ToFloat(XMMatrixTranspose(baseDir)));
		}

		SECTION("Rotation")
		{
			CHECK_MATRIX4X4(FMat4::xRotation(32), 
				ToFloat(XMMatrixRotationX((float)Utilities::DegreesToRadians(32))));
			CHECK_MATRIX4X4(FMat4::yRotation(108),
				ToFloat(XMMatrixRotationY((float)Utilities::DegreesToRadians(108))));
			CHECK_MATRIX4X4(FMat4::zRotation(213),
				ToFloat(XMMatrixRotationZ((float)Utilities::DegreesToRadians(213))));
		}

		SECTION("Look")
		{
			XMFLOAT3 f1(4.3f, 2.5f, 8.f);
			XMFLOAT3 f2(8.3f, 4.2f, 5.f);
			XMFLOAT3 f3(.0f, 1.f, 0.f);
			XMVECTOR vec1 = XMLoadFloat3(&f1);
			XMVECTOR vec2 = XMLoadFloat3(&f2);
			XMVECTOR vec3 = XMLoadFloat3(&f3);

			{
				CHECK_MATRIX4X4(FMat4::LookAtRH(FVec3(4.3f, 2.5f, 8.f), FVec3(8.3f, 4.2f, 5.f), FVec3::mUp),
					ToFloat(XMMatrixLookAtRH(vec1, vec2, vec3)));
			}

			{
				CHECK_MATRIX4X4(FMat4::LookAtLH(FVec3(4.3f, 2.5f, 8.f), FVec3(8.3f, 4.2f, 5.f), FVec3::mUp),
					ToFloat(XMMatrixLookAtLH(vec1, vec2, vec3)));
			}
		}

		SECTION("Perspective")
		{
			CHECK_MATRIX4X4(FMat4::PerspectiveRHDirect(45.f, 800.f / 600.f, .5f, 1000.f), 
				ToFloat(XMMatrixPerspectiveFovRH((float)Utilities::DegreesToRadians(45.f), 800.f / 600.f, .5f, 1000.f)));
		}
	}
}