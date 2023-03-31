#pragma once
#include <limits>
#include <algorithm>
#include "Vec3.h"

#ifndef HALF_CIRCLE
#define HALF_CIRCLE 180.0f
#endif

#ifdef max
#undef max
#endif // max

namespace lm
{
	template <typename T> class Vec4
	{
	private:
		T mX;
		T mY;
		T mZ;
		T mW;

	public:
		static const T mUnitVal;
		static const Vec4<T> mZero;
		static const Vec4<T> mOne;
		static const Vec4<T> mUp;
		static const Vec4<T> mDown;
		static const Vec4<T> mLeft;
		static const Vec4<T> mRight;

		Vec4()
		{
			this->mX = 0;
			this->mY = 0;
			this->mZ = 0;
			this->mW = 0;
		}

		Vec4(const T pInit)
		{
			this->mX = pInit;
			this->mY = pInit;
			this->mZ = pInit;
			this->mW = pInit;
		}

		Vec4(const T pX, const T pY, const T pZ, const T pW)
		{
			this->mX = pX;
			this->mY = pY;
			this->mZ = pZ;
			this->mW = pW;
		}

		//Under directXMath, XMFLOAT4.w is initialized at 0
		Vec4(const Vec3<T>& pVec3, float pW = 0)
		{
			this->mX = pVec3.X();
			this->mY = pVec3.Y();
			this->mZ = pVec3.Z();
			this->mW = pW;
		}

		Vec4(const Vec4<T>& pOther)
		{
			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;
			this->mW = pOther.mW;
		}

		Vec4(Vec4<T>&& pOther) noexcept
		{
			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);
			this->mW = std::move(pOther.mW);
		}

		Vec4& operator=(const Vec4<T>& pOther)
		{
			if (this == &pOther)
				return *this;

			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;
			this->mW = pOther.mW;

			return *this;
		}

		Vec4& operator=(Vec4<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);
			this->mW = std::move(pOther.mW);

			return *this;
		}

		T& X()
		{
			return this->mX;
		}

		T& Y()
		{
			return this->mY;
		}

		T& Z()
		{
			return this->mZ;
		}

		T& W()
		{
			return this->mW;
		}

		const T X() const
		{
			return this->mX;
		}

		const T Y() const
		{
			return this->mY;
		}

		const T Z() const
		{
			return this->mZ;
		}

		const T W() const
		{
			return this->mW;
		}

		const T Length() const
		{
			return sqrt((this->mX * this->mX) +
				(this->mY * this->mY) +
				(this->mZ * this->mZ) +
				(this->mW * this->mW));
		}

		const T Length2() const
		{
			return (this->mX * this->mX) +
				(this->mY * this->mY) + 
				(this->mZ * this->mZ) +
				(this->mW * this->mW);
		}

		bool IsLongerThan(Vec4<T> const& pOther) const	// return true if this vector Length2 is greater than the other
		{
			return this->Length() > pOther.Length();
		}

		bool IsShorterThan(Vec4<T> const& pOther) const	// return true if this vector Length2 is less than the other
		{
			return this->Length() < pOther.Length();
		}

		const T DistanceFrom(Vec4<T> const& pOther) const // return distance between 2 points
		{
			return sqrt((pOther.mX - this.mX) * (pOther.mX - this.mX) +
				(pOther.mY - this.mY) * (pOther.mY - this.mY) +
				(pOther.mZ - this.mZ) * (pOther.mZ - this.mZ) +
				(pOther.mW - this.mW) * (pOther.mW - this.mW));
		}

		const bool IsUnit() const
		{
			return std::abs(this->Length() - Vec4<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - Vec4<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(Vec4<T>::mUnitVal));
		}

		const T DotProduct(const Vec4<T>& pOther) const
		{
			return (this->mX * pOther.mX) +
				(this->mY * pOther.mY) + 
				(this->mZ * pOther.mZ) +
				(this->mW * pOther.mW);
		}

		 /* ----- 
		 In 4D, Orthogonal is more relevant that perpendicular
		 -----*/

		/*Vec4<T> Perpendicular(const Vec4<T>& pOther) const
		{
			const Vec4<T> proj = this->ProjectOnTo(pOther);
			return Vec4<T>(this->mX - proj.mX,
				this->mY - proj.mY,
				this->mZ - proj.mZ,
				this->mW - proj.mW);
		}*/



		/* -----
		Cross product is not well defined for 4D vector,
		OrthogonalVector" return a generalized cross-product for 4D vector
		if we want a "cross-product" for 4D dimension, we should do it with 3 4d vectors
		----- */

		const Vec4<T> OrthogonalVector() const // return a vector orthogonal to self
		{
			return Vec4<T>(this->mZ, this->mW, -this->mX, -this->mY);
		}

		const T RadAngle(const Vec4<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);

			const T lengthA2 = this->Length2();
			const T lengthB2 = pOther.Length2();
			const T length2Product = lengthA2 * lengthB2;

			const T length = sqrt(length2Product);

			return std::acos(product / length);
		}

		const T DegAngle(const Vec4<T>& pOther) const
		{
			return Utilities::RadiansToDegrees(this->RadAngle(pOther));
		}

		/* -----
		Cannot project 4d vector on another
		-----*/

	/*	Vec4<T> ProjectOnTo(const Vec4<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);
			const T length = pOther.Length2();
			const T division = product / length;

			return Vec4<T>(division * pOther.mX,
				division * pOther.mY,
				division * pOther.mZ,
				division * pOther.mW);
		}*/

		Vec4<T> Reflexion(const Vec4<T>& pOther) const
		{
			Vec4<T> tmp = pOther;
			if (!pOther.IsUnit())
			{
				/*			Vec4<T> proj = this->ProjectOnTo(pOther);
							proj *= 2;
							return Vec4<T>(this->mX - proj.mX,
								this->mY - proj.mY,
								this->mZ - proj.mZ,
								this->mW - proj.mW);*/

				tmp = pOther.Normalize();
			}

			const T product = this->DotProduct(tmp);
			const T doubledProduct = 2 * product;
			const Vec4<T> vecProduct = doubledProduct * tmp;

			return Vec4<T>(this->mX - vecProduct.mX,
				this->mY - vecProduct.mY,
				this->mZ - vecProduct.mZ,
				this->mW - vecProduct.mW);
		}

		Vec4<T> Add(const Vec4<T>& pOther) const
		{
			return Vec4<T>(this->mX + pOther.mX, this->mY + pOther.mY, this->mZ + pOther.mZ,
				this->mW + pOther.mW);
		}

		void AddSelf(const Vec4<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			this->mZ += pOther.mZ;
			this->mW += pOther.mW;
		}

		Vec4<T> Scale(const T pScale) const
		{
			return Vec4<T>(this->mX * pScale,
				this->mY * pScale,
				this->mZ * pScale,
				this->mW * pScale);
		}

		void ScaleSelf(const T pScale)
		{
			this->mX *= pScale;
			this->mY *= pScale;
			this->mZ *= pScale;
			this->mW *= pScale;
		}

		Vec4<T> Normalize() const
		{
			T length = this->Length();
			return Vec4<T>(this->mX / length,
				this->mY / length,
				this->mZ / length,
				this->mW / length);
		}

		void NormalizeSelf()
		{
			T length = this->Length();
			this->mX /= length;
			this->mY /= length;
			this->mZ /= length;
			this->mW /= length;
		}

		T& operator[](const int pIdx)
		{
			switch (pIdx)
			{
			case 0:		return mX;
			case 1:		return mY;
			case 2:		return mZ;
			case 3:		return mW;

			case 'x':	return mX;
			case 'y':	return mY;
			case 'z':	return mZ;
			case 'w':	return mW;

			default:	return mX;
			}
		}

		const T operator[](const int pIdx) const
		{
			switch (pIdx)
			{
			case 0:		return mX;
			case 1:		return mY;
			case 2:		return mZ;
			case 3:		return mW;

			case 'x':	return mX;
			case 'y':	return mY;
			case 'z':	return mZ;
			case 'w':	return mW;

			default:	return mX;
			}
		}

		T& operator[](const char* pIdx)
		{
			switch (pIdx[0])
			{
			case 'x': return mX;
			case 'y': return mY;
			case 'z': return mZ;
			case 'w': return mW;

			default: return mX;
			}
		}

		const T operator[](const char* pIdx) const
		{
			switch (pIdx[0])
			{
			case 'x': return mX;
			case 'y': return mY;
			case 'z': return mZ;
			case 'w': return mW;

			default: return  mX;
			}
		}

		const bool operator==(const Vec4<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			return std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(pOther.Length()));
		}

		const bool operator!=(const Vec4<T>& pOther) const
		{
			return !(*this == pOther);
		}

		const bool operator<(const Vec4<T>& pOther) const
		{
			return this->Length() < pOther.Length();
		}

		const bool operator<=(const Vec4<T>& pOther) const
		{
			return this->Length() < pOther.Length() || (*this == pOther);
		}

		const bool operator>(const Vec4<T>& pOther) const
		{
			return !(*this < pOther);
		}

		const bool operator>=(const Vec4<T>& pOther) const
		{
			return !(*this <= pOther);
		}

		Vec4<T> operator+(const Vec4<T>& pOther) const
		{
			return Vec4(this->mX + pOther.mX, this->mY + pOther.mY, this->mZ + pOther.mZ, this->mW + pOther.mW);
		}

		void operator+=(const Vec4<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			this->mZ += pOther.mZ;
			this->mW += pOther.mW;
		}

		Vec4<T> operator-(const Vec4<T>& pOther) const
		{
			return Vec4(this->mX - pOther.mX,
				this->mY - pOther.mY,
				this->mZ - pOther.mZ,
				this->mW - pOther.mW);
		}

		void operator-=(const Vec4<T>& pOther)
		{
			this->mX -= pOther.mX;
			this->mY -= pOther.mY;
			this->mZ -= pOther.mZ;
			this->mW -= pOther.mW;
		}

		Vec4<T> operator-() const
		{
			return Vec4(-this->mX, -this->mY, -this->mZ, this->mW);
		}

		Vec4<T> operator*(const T pValue) const
		{
			return this->Scale(pValue);
		}

		void operator*=(const T pValue)
		{
			*this = this->Scale(pValue);
		}

		Vec4<T> operator*(const Vec4<T>& pOther) const
		{
			return Vec4<T>(this->mX * pOther.mX,
				this->mY * pOther.mY,
				this->mZ * pOther.mZ,
				this->mW * pOther.mW);
		}

		void operator*=(const Vec4<T>& pOther)
		{
			this->mX *= pOther.mX;
			this->mY *= pOther.mY;
			this->mZ *= pOther.mZ;
			this->mW *= pOther.mW;
		}

		Vec4<T> operator/(const T pValue) const
		{
			return Vec4(this->mX / pValue,
				this->mY / pValue,
				this->mZ / pValue,
				this->mW / pValue);
		}

		void operator/=(const T pValue)
		{
			this->mX /= pValue;
			this->mY /= pValue;
			this->mZ /= pValue;
			this->mW /= pValue;
		}

		Vec4<T> operator/(const Vec4<T>& pOther) const
		{
			return Vec4<T>(this->mX / pOther.mX,
				this->mY / pOther.mY,
				this->mZ / pOther.mZ,
				this->mW / pOther.mW);
		}

		void operator/=(const Vec4<T>& pOther)
		{
			this->mX /= pOther.mX;
			this->mY /= pOther.mY;
			this->mZ /= pOther.mZ;
			this->mW /= pOther.mW;
		}

		void operator++(int)
		{
			T length = this->Length();
			this->mX *= (length + 1) / length;
			this->mY *= (length + 1) / length;
			this->mZ *= (length + 1) / length;
			this->mW *= (length + 1) / length;
		}

		void operator--(int)
		{
			T length = this->Length();
			this->mX *= (length - 1) / length;
			this->mY *= (length - 1) / length;
			this->mZ *= (length - 1) / length;
			this->mW *= (length - 1) / length;
		}

		void HomogenizeSelf()
		{
			if (this->mW == 0)
				return;

			this->mX /= this->mW;
			this->mY /= this->mW;
			this->mZ /= this->mW;
		}

		Vec4<T> Homogenize()
		{
			if (this->mW == 0)
				return Vec4<T>();

			return Vec4<T>(this->mX / this->mW,
				this->mY / this->mW,
				this->mZ / this->mW,
				1);
		}

		float GetMagnitude()
		{
			return this->Length();
		}
	};

	template<class T> Vec4<T> operator*(const double pValue, const Vec4<T>& pOther)
	{
		return pOther * T(pValue);
	}

	template<class T> Vec4<T> operator/(const double pValue, const Vec4<T>& pOther)
	{
		return Vec4<T>(T(pValue / pOther.X()),
			T(pValue / pOther.Y()),
			T(pValue / pOther.Z()),
			T(pValue / pOther.W()));
	}

	template<class T> const T operator,(const Vec4<T>& pOther1, const Vec4<T>& pOther2)
	{
		return pOther1.DotProduct(pOther2);
	}

	template<class T> const Vec4<T> Vec4<T>::mZero = Vec4();
	template<class T> const Vec4<T> Vec4<T>::mOne = Vec4(1);
	template<class T> const Vec4<T> Vec4<T>::mUp(0, 1, 0, 0);
	template<class T> const Vec4<T> Vec4<T>::mDown(0, -1, 0, 0);
	template<class T> const Vec4<T> Vec4<T>::mLeft(-1, 0, 0, 0);
	template<class T> const Vec4<T> Vec4<T>::mRight(1, 0, 0, 0);
	template<class T> const T Vec4<T>::mUnitVal = 1;

	typedef Vec4<float>		FVec4;
	typedef Vec4<double>	DVec4;
}