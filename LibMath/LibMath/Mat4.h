#pragma once
//#include <Windows.h>
#include "Vec4.h"
#include "Mat3.h"

namespace lm
{
	template <typename T> class Mat4
	{
	private:
		Vec4<T> mMatrix[4];

	public:
		static const Mat4<T> mIdentity;

		Vec4<T>& GetRaw0() { return mMatrix[0]; }
		Vec4<T>& GetRaw1() { return mMatrix[1]; }
		Vec4<T>& GetRaw2() { return mMatrix[2]; }
		Vec4<T>& GetRaw3() { return mMatrix[3]; }

		Mat4()
		{
			this->mMatrix[0] = Vec4<T>(0, 0, 0, 0);
			this->mMatrix[1] = Vec4<T>(0, 0, 0, 0);
			this->mMatrix[2] = Vec4<T>(0, 0, 0, 0);
			this->mMatrix[3] = Vec4<T>(0, 0, 0, 0);
		}

		Mat4(const T pInit)
		{
			this->mMatrix[0] = Vec4<T>(pInit, 0, 0, 0);
			this->mMatrix[1] = Vec4<T>(0, pInit, 0, 0);
			this->mMatrix[2] = Vec4<T>(0, 0, pInit, 0);
			this->mMatrix[3] = Vec4<T>(0, 0, 0, pInit);
		}

		Mat4(const Vec4<T>& pVec1, const Vec4<T>& pVec2, const Vec4<T>& pVec3, const Vec4<T>& pVec4)
		{
			this->mMatrix[0] = pVec1;
			this->mMatrix[1] = pVec2;
			this->mMatrix[2] = pVec3;
			this->mMatrix[3] = pVec4;
		}

		Mat4(const T& p00, const T& p01, const T& p02, const T& p03,
			const T& p10, const T& p11, const T& p12, const T& p13,
			const T& p20, const T& p21, const T& p22, const T& p23,
			const T& p30, const T& p31, const T& p32, const T& p33)
		{
			this->mMatrix[0][0] = p00;
			this->mMatrix[0][1] = p01;
			this->mMatrix[0][2] = p02;
			this->mMatrix[0][3] = p03;
			this->mMatrix[1][0] = p10;
			this->mMatrix[1][1] = p11;
			this->mMatrix[1][2] = p12;
			this->mMatrix[1][3] = p13;
			this->mMatrix[2][0] = p20;
			this->mMatrix[2][1] = p21;
			this->mMatrix[2][2] = p22;
			this->mMatrix[2][3] = p23;
			this->mMatrix[3][0] = p30;
			this->mMatrix[3][1] = p31;
			this->mMatrix[3][2] = p32;
			this->mMatrix[3][3] = p33;
		}

		Mat4(const Mat4<T>& pOther)
		{
			for (unsigned int i = 0; i < 4; i++)
				this->mMatrix[i] = pOther.mMatrix[i];
		}

		Mat4(Mat4<T>&& pOther) noexcept
		{
			for (unsigned int i = 0; i < 4; i++)
				this->mMatrix[i] = std::move(pOther.mMatrix[i]);
		}

		Mat4<T>& operator=(const Mat4<T>& pOther)
		{
			if (this == &pOther)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->mMatrix[i] = pOther.mMatrix[i];

			return *this;
		}

		Mat4<T>& operator=(Mat4<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			for (unsigned int i = 0; i < 4; i++)
				this->mMatrix[i] = std::move(pOther.mMatrix[i]);

			return *this;
		}

		const Vec4<T> operator[](int pIdx) const
		{
			if (pIdx >= 0 && pIdx < 4)
				return this->mMatrix[pIdx];
			return this->mMatrix[0];
		}

		Vec4<T>& operator[](int pIdx)
		{
			if (pIdx >= 0 && pIdx < 4)
				return this->mMatrix[pIdx];
			return this->mMatrix[0];
		}

		Mat4<T> DotProduct(const Mat4<T>& pOther) const
		{
			Mat4<T> newMat4;

			for (unsigned int i = 0; i < 4; i++)
			{
				Vec4<T> vec4;
				for (unsigned int j = 0; j < 4; j++)
				{
					vec4[j] = this->mMatrix[i][0] * pOther.mMatrix[0][j]
						+ this->mMatrix[i][1] * pOther.mMatrix[1][j]
						+ this->mMatrix[i][2] * pOther.mMatrix[2][j]
						+ this->mMatrix[i][3] * pOther.mMatrix[3][j];
				}
				newMat4.mMatrix[i] = vec4;
			}

			return newMat4;
		}

		static Mat4<T> CreateTransformMatrix(const Vec3<T>& pPosition, const Vec3<T>& pRotation, const Vec3<T>& pScaleVec)
		{
			return Translation(pPosition) * zRotation(pRotation.Z()) * xRotation(pRotation.X()) * yRotation(pRotation.Y()) * Scale(pScaleVec);
		}

		static Mat4<T> createView(const Vec3<T>& pPosition, const Vec3<T>& pRotation)
		{
			return Translation(pPosition) * xRotation(pRotation.X()) * yRotation(pRotation.Y());
		}

		static Mat4<T> LookAtRH(lm::Vec3<T> const& pEye, lm::Vec3<T> const& pFocus, lm::Vec3<T> const& pUp)
		{
			lm::Vec3<T>  f = (pFocus - pEye).Normalize();
			lm::Vec3<T>  u = pUp.Normalize();
			lm::Vec3<T>  r = f.CrossProduct(u).Normalize();
			u = r.CrossProduct(f);

			lm::Mat4<T> Result = Mat4::mIdentity;
			Result[0][0] = r.X();
			Result[1][0] = r.Y();
			Result[2][0] = r.Z();

			Result[0][1] = u.X();
			Result[1][1] = u.Y();
			Result[2][1] = u.Z();

			Result[0][2] = -f.X();
			Result[1][2] = -f.Y();
			Result[2][2] = -f.Z();

			Result[3][0] = -(r.DotProduct(pEye));
			Result[3][1] = -(u.DotProduct(pEye));
			Result[3][2] = f.DotProduct(pEye);
			return Result;
		}

		static Mat4<T> LookAtLH(lm::Vec3<T> const& pEye, lm::Vec3<T> const& pFocus, lm::Vec3<T> const& pUp)
		{
			lm::Vec3<T>  f = (pFocus - pEye).Normalize();
			lm::Vec3<T>  u = pUp.Normalize();
			lm::Vec3<T>  r = -f.CrossProduct(u).Normalize();
			u = -r.CrossProduct(f);

			lm::Mat4<T> Result = Mat4::mIdentity;
			Result[0][0] = r.X();
			Result[1][0] = r.Y();
			Result[2][0] = r.Z();

			Result[0][1] = u.X();
			Result[1][1] = u.Y();
			Result[2][1] = u.Z();

			Result[0][2] = f.X();
			Result[1][2] = f.Y();
			Result[2][2] = f.Z();

			Result[3][0] = -r.DotProduct(pEye);
			Result[3][1] = -u.DotProduct(pEye);
			Result[3][2] = -f.DotProduct(pEye);
			return Result;
		}

		static Mat4<T> LookAtRotation(lm::Vec3<T> const& pEye, lm::Vec3<T> const& pFocus,
			lm::Vec3<T> const& pUp)
		{
			lm::Vec3<T>  f = (pFocus - pEye).Normalize();
			lm::Vec3<T>  u = pUp.Normalize();
			lm::Vec3<T>  r = f.CrossProduct(u).Normalize();
			u = r.CrossProduct(f);

			lm::Mat4<T> Result = Mat4::mIdentity;
			Result[0][0] = r.X(); //
			Result[1][0] = r.Y(); //
			Result[2][0] = r.Z(); //

			Result[0][1] = u.X();
			Result[1][1] = u.Y();
			Result[2][1] = u.Z();

			Result[0][2] = -f.X(); //
			Result[1][2] = -f.Y();
			Result[2][2] = -f.Z();//

			Result[3][0] = -pEye.X(); //
			Result[3][1] = -pEye.Y();
			Result[3][2] = -pEye.Z();//
			return Result;
		}

		const T Determinant() const
		{
			Mat3<T> cofactor0{
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> cofactor1{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> cofactor2{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> cofactor3{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3] };

			return mMatrix[0][0] * cofactor0.Determinant() -
				mMatrix[1][0] * cofactor1.Determinant() +
				mMatrix[2][0] * cofactor2.Determinant() -
				mMatrix[3][0] * cofactor3.Determinant();
		}

		Mat4<T> Adjugate() const
		{
			Mat3<T> sub00{
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub01{
				mMatrix[1][0], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][0], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][0], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub02{
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][3],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][3],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][3] };

			Mat3<T> sub03{
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][2],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][2],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][2] };

			Mat3<T> sub10{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub11{
				mMatrix[0][0], mMatrix[0][2], mMatrix[0][3],
				mMatrix[2][0], mMatrix[2][2], mMatrix[2][3],
				mMatrix[3][0], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub12{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][3],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][3],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][3] };

			Mat3<T> sub13{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][2],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][2],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][2] };

			Mat3<T> sub20{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[3][1], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub21{
				mMatrix[0][0], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][0], mMatrix[1][2], mMatrix[1][3],
				mMatrix[3][0], mMatrix[3][2], mMatrix[3][3] };

			Mat3<T> sub22{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][3],
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][3],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][3] };

			Mat3<T> sub23{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][2],
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][2],
				mMatrix[3][0], mMatrix[3][1], mMatrix[3][2] };

			Mat3<T> sub30{
				mMatrix[0][1], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][1], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][1], mMatrix[2][2], mMatrix[2][3] };

			Mat3<T> sub31{
				mMatrix[0][0], mMatrix[0][2], mMatrix[0][3],
				mMatrix[1][0], mMatrix[1][2], mMatrix[1][3],
				mMatrix[2][0], mMatrix[2][2], mMatrix[2][3] };

			Mat3<T> sub32{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][3],
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][3],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][3] };

			Mat3<T> sub33{
				mMatrix[0][0], mMatrix[0][1], mMatrix[0][2],
				mMatrix[1][0], mMatrix[1][1], mMatrix[1][2],
				mMatrix[2][0], mMatrix[2][1], mMatrix[2][2] };

			return Mat4<T>{
					sub00.Determinant(), -sub01.Determinant(), sub02.Determinant(), -sub03.Determinant(),
					-sub10.Determinant(), sub11.Determinant(), -sub12.Determinant(), sub13.Determinant(),
					sub20.Determinant(), -sub21.Determinant(), sub22.Determinant(), -sub23.Determinant(),
					-sub30.Determinant(), sub31.Determinant(), -sub32.Determinant(), sub33.Determinant() 
			}.Transpose();
		}

		Mat4<T> Inverse() const
		{
			const T det = this->Determinant();
			const T div = 1.f / det;

			return this->Adjugate() * div;
		}

		Mat4<T> Transpose() const
		{
			return Mat4<T>{
				mMatrix[0][0], mMatrix[1][0], mMatrix[2][0], mMatrix[3][0],
				mMatrix[0][1], mMatrix[1][1], mMatrix[2][1], mMatrix[3][1],
				mMatrix[0][2], mMatrix[1][2], mMatrix[2][2], mMatrix[3][2],
				mMatrix[0][3], mMatrix[1][3], mMatrix[2][3], mMatrix[3][3]};
		}

		static Mat4<T> Translation(const Vec3<T>& pTranslation)
		{
			Mat4<T> translate;
			translate[3][0] = pTranslation.X();
			translate[3][1] = pTranslation.Y();
			translate[3][2] = pTranslation.Z();
			return translate;
		}

		void Translate(const Vec3<T>& pTranslate)
		{
			this *= Translation(pTranslate);
		}

		Mat4<T> Scale(const T& pScale) const
		{
			Mat4<T> matrixScale = *this;
			matrixScale[0] *= pScale;
			matrixScale[1] *= pScale;
			matrixScale[2] *= pScale;
			matrixScale[3] *= pScale;
			return matrixScale;
		}

		void ScaleSelf(const T& pScale)
		{
			this->mMatrix[0] *= pScale;
			this->mMatrix[1] *= pScale;
			this->mMatrix[2] *= pScale;
			this->mMatrix[3] *= pScale;
		}

		static Mat4<T> xRotation(float pAngle)
		{
			float rad = float(Utilities::DegreesToRadians(double(pAngle)));

			Mat4<T> matrixScale = mIdentity;
			matrixScale[1][1] = std::cosf(rad);
			matrixScale[1][2] = std::sinf(rad);

			matrixScale[2][1] = -std::sinf(rad);
			matrixScale[2][2] = std::cosf(rad);

			return matrixScale;
		}

		static Mat4<T> yRotation(float pAngle)
		{
			float rad = float(Utilities::DegreesToRadians(double(pAngle)));

			Mat4<T> matrixRotation = mIdentity;
			matrixRotation[0][0] = std::cosf(rad);
			matrixRotation[0][2] = -std::sinf(rad);

			matrixRotation[2][0] = std::sinf(rad);
			matrixRotation[2][2] = std::cosf(rad);

			return matrixRotation;
		}

		static Mat4<T> zRotation(float pAngle)
		{
			float rad = float(Utilities::DegreesToRadians(double(pAngle)));

			Mat4<T> matrixScale = mIdentity;
			matrixScale[0][0] = std::cosf(rad);
			matrixScale[0][1] = std::sinf(rad);

			matrixScale[1][0] = -std::sinf(rad);
			matrixScale[1][1] = std::cosf(rad);

			return matrixScale;
		}

		static Mat4<T> PerspectiveRH(const float pFov, const float pAspect, const float pNear, const float pFar)
		{
			const float tangent = tanf(pFov / 2.0f * (float)M_PI / 180.0f);
			const float height = pNear * tangent;
			const float width = height * pAspect;

			return CreateFrustum(-width, width, -height, height, pNear, pFar);
		}

		static Mat4<T> CreateFrustum(const float pLeft, const float pRight, const float pBottom,
			const float pTop, const float pNear, const float pFar)
		{
			const float maxView = 2.0f * pNear;
			const float width = pRight - pLeft;
			const float height = pTop - pBottom;
			const float zRange = pFar - pNear;

			Mat4<T> Frustum;
			Frustum.mMatrix[0][0] = maxView / width;
			Frustum.mMatrix[1][1] = maxView / height;
			Frustum.mMatrix[2][0] = (pRight + pLeft) / width;
			Frustum.mMatrix[2][1] = (pTop + pBottom) / height;
			Frustum.mMatrix[2][2] = (-pFar - pNear) / zRange;
			Frustum.mMatrix[2][3] = -1.0f;
			Frustum.mMatrix[3][2] = (-maxView * pFar) / zRange;
			Frustum.mMatrix[3][3] = 0.0f;

			return Frustum;
		}
		
		static Mat4<T> PerspectiveRHDirect(const float pFov, const float pAspect, const float pNear, const float pFar)
		{
			const float tangent = tanf(pFov / 2.0f * (float)M_PI / 180.0f);
			const float height = pNear * tangent;
			const float width = height * pAspect;

			return CreateFrustumDirect(-width, width, -height, height, pNear, pFar);
		}

		static Mat4<T> CreateFrustumDirect(const float pLeft, const float pRight, const float pBottom,
			const float pTop, const float pNear, const float pFar)
		{
			const float maxView = 2.0f * pNear;
			const float width = pRight - pLeft;
			const float height = pTop - pBottom;
			const float zRange = pFar / (pNear - pFar);

			Mat4<T> Frustum;
			Frustum.mMatrix[0][0] = maxView / width;
			Frustum.mMatrix[1][1] = maxView / height;
			Frustum.mMatrix[2][0] = (pRight + pLeft) / width;
			Frustum.mMatrix[2][1] = (pTop + pBottom) / height;
			Frustum.mMatrix[2][2] = zRange;
			Frustum.mMatrix[2][3] = -1.0f;
			Frustum.mMatrix[3][2] = pNear * zRange;
			Frustum.mMatrix[3][3] = 0.0f;

			return Frustum;
		}

		bool IsIdentity() const
		{
			return *this == mIdentity;
		}

		lm::Vec3<T> ExtractScale()
		{
			lm::Vec3<T> xScalar(this->mMatrix[0].X(), this->mMatrix[0].Y(), this->mMatrix[0].Z());
			lm::Vec3<T> yScalar(this->mMatrix[1].X(), this->mMatrix[1].Y(), this->mMatrix[1].Z());
			lm::Vec3<T> zScalar(this->mMatrix[2].X(), this->mMatrix[2].Y(), this->mMatrix[2].Z());
			float x = xScalar.Length();
			float y = yScalar.Length();
			float z = zScalar.Length();
			lm::Vec3<T> scalar(x, y, z);
			return scalar;
		}

		lm::Vec3<T> ExtractRotation(lm::Vec3<T> pScale)
		{
			float theta1, theta2, gamma1, gamma2, phi1, phi2;
			Mat4<T> rotationMatrix(Vec4<T>(this->mMatrix[0].X() / pScale.X(), this->mMatrix[0].Y() / pScale.X(),
				this->mMatrix[0].Z() / pScale.X(), 0),
				Vec4<T>(this->mMatrix[1].X() / pScale.Y(), this->mMatrix[1].Y() / pScale.Y(), this->mMatrix[1].Z() / pScale.Y(), 0),
				Vec4<T>(this->mMatrix[2].X() / pScale.Z(), this->mMatrix[2].Y() / pScale.Z(), this->mMatrix[2].Z() / pScale.Z(), 0),
				Vec4<T>(0, 0, 0, 1));

			/*if (rotationMatrix[2].X() != 1 || rotationMatrix[2].X() != -1)
			{
				theta1 = -asin(rotationMatrix[2].X());
				theta2 = M_PI - theta1;

				gamma1 = atan2(rotationMatrix[2].Y() / cos(theta1), rotationMatrix[2].Z() / cos(theta1));
				gamma2 = atan2(rotationMatrix[2].Y() / cos(theta2), rotationMatrix[2].Z() / cos(theta2));

				phi1 = atan2(rotationMatrix[1].X() / cos(theta1), rotationMatrix[0].X() / cos(theta1));
				phi2 = atan2(rotationMatrix[1].X() / cos(theta2), rotationMatrix[0].X() / cos(theta2));
			}
			else
			{
				phi1 = 0;
				if (rotationMatrix[2].X() != -1)
				{
					theta1 = M_PI / 2;
					gamma1 = atan2(rotationMatrix[0].Y(), rotationMatrix[0].Z());
				}
				else
				{
					theta1 = -M_PI / 2;
					gamma1 = atan2(-rotationMatrix[0].Y(), -rotationMatrix[0].Z());
				}
			}*/

			if (rotationMatrix[1].Z() < 1)
			{
				if (rotationMatrix[1].Z() > -1)
				{
					theta1 = asin(rotationMatrix[1].Z());
					gamma1 = atan2(-rotationMatrix[1].X(), rotationMatrix[1].Y());
					phi1 = atan2(-rotationMatrix[0].Z(), rotationMatrix[2].Z());
				}
				else
				{
					theta1 = -M_PI / 2;
					gamma1 = -atan2(rotationMatrix[2].X(), rotationMatrix[0].Y());
					phi1 = 0;
				}
			}
			else
			{
				theta1 = M_PI / 2;
				gamma1 = atan2(rotationMatrix[2].X(), rotationMatrix[0].Y());
				phi1 = 0;
			}

			lm::FVec3 rotateVector(theta1 * (180 / M_PI), gamma1 * (180 / M_PI), phi1 * (180 / M_PI));
			return rotateVector;
		}

		Mat4<T> operator*(const float pScale) const
		{
			return this->Scale(pScale);
		}

		void operator*=(const float pScale)
		{
			this->ScaleSelf(pScale);
		}

		Mat4<T> operator*(const Mat4<T>& pOther) const
		{
			return this->DotProduct(pOther);
		}

		void operator*=(const Mat4<T>& pOther)
		{
			*this = this->DotProduct(pOther);
		}

		Vec4<T> operator*(const Vec4<T>& pVec4) const
		{
			Vec4<T> newVec4;
			newVec4.X() = (this->mMatrix[0][0] * pVec4.X()) + (this->mMatrix[1].X() * pVec4.Y()) +
				(this->mMatrix[2].X() * pVec4.Z()) + (this->mMatrix[3].X() * pVec4.W());
			newVec4.Y() = (this->mMatrix[0][1] * pVec4.X()) + (this->mMatrix[1].Y() * pVec4.Y()) +
				(this->mMatrix[2].Y() * pVec4.Z()) + (this->mMatrix[3].Y() * pVec4.W());
			newVec4.Z() = (this->mMatrix[0].Z() * pVec4.X()) + (this->mMatrix[1].Z() * pVec4.Y()) +
				(this->mMatrix[2].Z() * pVec4.Z()) + (this->mMatrix[3].Z() * pVec4.W());
			newVec4.W() = (this->mMatrix[0].W() * pVec4.X()) + (this->mMatrix[1].W() * pVec4.Y()) +
				(this->mMatrix[2].W() * pVec4.Z()) + (this->mMatrix[3].W() * pVec4.W());
			return newVec4;
		}

		const bool operator==(const Mat4<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			for (unsigned int i = 0; i < 4; i++)
				if (this->mMatrix[i] != pOther.mMatrix[i])
					return false;

			return true;
		}

		const bool operator!=(const Mat4<T>& pOther) const
		{
			return !(*this == pOther);
		}
	};

	template<class T> const Mat4<T> Mat4<T>::mIdentity = Mat4<T>(1);

	typedef Mat4<float> FMat4;
	typedef Mat4<double> DMat4;
}