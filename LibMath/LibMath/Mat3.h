#pragma once

#include "Vec3.h"

namespace lm
{
	template <typename T> class Mat3
	{
	private:
		Vec3<T> mMatrix[3];

	public:
		static const Mat3<T> mIdentity;

		Mat3()
		{
			this->mMatrix[0] = Vec3<T>(0, 0, 0);
			this->mMatrix[1] = Vec3<T>(0, 0, 0);
			this->mMatrix[2] = Vec3<T>(0, 0, 0);
		}

		Mat3(const T pInit)
		{
			this->mMatrix[0] = Vec3<T>(pInit, 0, 0);
			this->mMatrix[1] = Vec3<T>(0, pInit, 0);
			this->mMatrix[2] = Vec3<T>(0, 0, pInit);
		}

		Mat3(const Vec3<T>& pVec1, const Vec3<T>& pVec2, const Vec3<T>& pVec3)
		{
			this->mMatrix[0] = pVec1;
			this->mMatrix[1] = pVec2;
			this->mMatrix[2] = pVec3;
		}

		Mat3(const T& p00, const T& p01, const T& p02,
			const T& p10, const T& p11, const T& p12,
			const T& p20, const T& p21, const T& p22)
		{
			mMatrix[0][0] = p00;
			mMatrix[0][1] = p01;
			mMatrix[0][2] = p02;
			mMatrix[1][0] = p10;
			mMatrix[1][1] = p11;
			mMatrix[1][2] = p12;
			mMatrix[2][0] = p20;
			mMatrix[2][1] = p21;
			mMatrix[2][2] = p22;
		}

		Mat3(const Mat3<T>& pOther)
		{
			for (unsigned int i = 0; i < 3; i++)
				this->mMatrix[i] = pOther.mMatrix[i];
		}

		Mat3(Mat3<T>&& pOther) noexcept
		{
			for (unsigned int i = 0; i < 3; i++)
				this->mMatrix[i] = std::move(pOther.mMatrix[i]);
		}

		Mat3<T>& operator=(const Mat3<T>& pOther)
		{
			if (this == &pOther)
				return *this;

			for (unsigned int i = 0; i < 3; i++)
				this->mMatrix[i] = pOther.mMatrix[i];

			return *this;
		}

		Mat3<T>& operator=(Mat3<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			for (unsigned int i = 0; i < 3; i++)
				this->mMatrix[i] = std::move(pOther.mMatrix[i]);

			return *this;
		}

		const Vec3<T> operator[](int pIdx) const
		{
			if (pIdx >= 0 && pIdx < 3)
				return mMatrix[pIdx];

			return mMatrix[0];
		}

		Vec3<T>& operator[](int pIdx)
		{
			if (pIdx >= 0 && pIdx < 3)
				return mMatrix[pIdx];

			return mMatrix[0];
		}

		Mat3<T> Scale(const float pScale) const
		{
			Mat3<T> mat3(*this);
			mat3[0] *= pScale;
			mat3[1] *= pScale;
			mat3[2] *= pScale;

			return mat3;
		}

		void ScaleSelf(const float pScale)
		{
			mMatrix[0] *= pScale;
			mMatrix[1] *= pScale;
			mMatrix[2] *= pScale;
		}

		Mat3<T> DotProduct(const Mat3<T>& pOther) const
		{
			Mat3<T> newMat3;

			for (unsigned int i = 0; i < 3; i++)
			{
				Vec3<T> vec3;
				for (unsigned int j = 0; j < 3; j++)
				{
					vec3[j] = this->mMatrix[i].X() * pOther.mMatrix[0][j]
						+ this->mMatrix[i].Y() * pOther.mMatrix[1][j]
						+ this->mMatrix[i].Z() * pOther.mMatrix[2][j];
				}
				newMat3.mMatrix[i] = vec3;
			}

			return newMat3;
		}

		Mat3<T> Transpose() const
		{
			return Mat3<T>(Vec3<T>(this->mMatrix[0].X(), this->mMatrix[1].X(), this->mMatrix[2].X()),
				Vec3<T>(this->mMatrix[0].Y(), this->mMatrix[1].Y(), this->mMatrix[2].Y()),
				Vec3<T>(this->mMatrix[0].Z(), this->mMatrix[1].Z(), this->mMatrix[2].Z()));
		}

		const T Determinant() const
		{
			return ((this->mMatrix[0].X() * ((this->mMatrix[1].Y() * this->mMatrix[2].Z()) -
				(this->mMatrix[1].Z() * this->mMatrix[2].Y()))) -
				this->mMatrix[0].Y() * ((this->mMatrix[1].X() * this->mMatrix[2].Z()) -
					(this->mMatrix[1].Z() * this->mMatrix[2].X()))) +
				(this->mMatrix[0].Z() * ((this->mMatrix[1].X() * this->mMatrix[2].Y()) -
					(this->mMatrix[1].Y() * this->mMatrix[2].X())));
		}

		Mat3<T> Minor() const
		{
			Mat3<T> newMat3;
			newMat3.mMatrix[0].X() = ((this->mMatrix[1].Y() * this->mMatrix[2].Z()) -
				(this->mMatrix[1].Z() * this->mMatrix[2].Y()));
			newMat3.mMatrix[0].Y() = ((this->mMatrix[1].X() * this->mMatrix[2].Z()) -
				(this->mMatrix[1].Z() * this->mMatrix[2].X()));
			newMat3.mMatrix[0].Z() = ((this->mMatrix[1].X() * this->mMatrix[2].Y()) -
				(this->mMatrix[1].Y() * this->mMatrix[2].X()));

			newMat3.mMatrix[1].X() = ((this->mMatrix[0].Y() * this->mMatrix[2].Z()) -
				(this->mMatrix[0].Z() * this->mMatrix[2].Y()));
			newMat3.mMatrix[1].Y() = ((this->mMatrix[0].X() * this->mMatrix[2].Z()) -
				(this->mMatrix[0].Z() * this->mMatrix[2].X()));
			newMat3.mMatrix[1].Z() = ((this->mMatrix[0].X() * this->mMatrix[2].Y()) -
				(this->mMatrix[0].Y() * this->mMatrix[2].X()));

			newMat3.mMatrix[2].X() = ((this->mMatrix[0].Y() * this->mMatrix[1].Z()) -
				(this->mMatrix[0].Z() * this->mMatrix[1].Y()));
			newMat3.mMatrix[2].Y() = ((this->mMatrix[0].X() * this->mMatrix[1].Z()) -
				(this->mMatrix[0].Z() * this->mMatrix[1].X()));
			newMat3.mMatrix[2].Z() = ((this->mMatrix[0].X() * this->mMatrix[1].Y()) -
				(this->mMatrix[0].Y() * this->mMatrix[1].X()));

			return newMat3;
		}

		Mat3<T> Cofactor() const
		{
			Mat3<T> min = this->Minor();
			min.mMatrix[0].Y() *= -1;
			min.mMatrix[1].X() *= -1;
			min.mMatrix[1].Z() *= -1;
			min.mMatrix[2].Y() *= -1;
			return min;
		}

		Mat3<T> Adjugate() const
		{
			Mat3<T> cof = this->Cofactor();
			cof = cof.Transpose();
			return cof;
		}

		Mat3<T> Inverse() const
		{
			const T determinant = this->Determinant();
			const T div = 1.0f / determinant;

			Mat3 mat3 = this->Adjugate();
			return Mat3<T>(Vec3<T>(mat3.mMatrix[0].X() * div, mat3.mMatrix[0].Y() * div, mat3.mMatrix[0].Z() * div),
				Vec3<T>(mat3.mMatrix[1].X() * div, mat3.mMatrix[1].Y() * div, mat3.mMatrix[1].Z() * div),
				Vec3<T>(mat3.mMatrix[2].X() * div, mat3.mMatrix[2].Y() * div, mat3.mMatrix[2].Z() * div));
		}

		bool IsIdentity() const
		{
			return *this == mIdentity;
		}

		Mat3<T> operator*(const float pScale) const
		{
			return this->Scale(pScale);
		}

		void operator*=(const float pScale)
		{
			this->ScaleSelf(pScale);
		}

		Mat3<T> operator*(const Mat3<T>& pOther) const
		{
			return this->DotProduct(pOther);
		}

		void operator*=(const Mat3<T>& pOther)
		{
			*this = this->DotProduct(pOther);
		}

		Vec3<T> operator*(const Vec3<T>& pVec3)
		{
			Vec3<T> newVec3;
			newVec3.X() = (this->mMatrix[0].X() * pVec3.X()) + (this->mMatrix[1].X() * pVec3.Y()) +
				(this->mMatrix[2].X() * pVec3.Z());
			newVec3.Y() = (this->mMatrix[0].Y() * pVec3.X()) + (this->mMatrix[1].Y() * pVec3.Y()) +
				(this->mMatrix[2].Y() * pVec3.Z());
			newVec3.Z() = (this->mMatrix[0].Z() * pVec3.X()) + (this->mMatrix[1].Z() * pVec3.Y()) +
				(this->mMatrix[2].Z() * pVec3.Z());
			return newVec3;
		}

		const bool operator==(const Mat3<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			for (unsigned int i = 0; i < 3; i++)
				if (this->mMatrix[i] != pOther.mMatrix[i])
					return false;

			return true;
		}

		const bool operator!=(const Mat3<T>& pOther) const
		{
			return !(*this == pOther);
		}
	};

	template<class T> const Mat3<T> Mat3<T>::mIdentity = Mat3<T>(1);

	typedef Mat3<float> FMat3;
	typedef Mat3<double> DMat3;
}