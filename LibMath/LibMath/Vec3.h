#pragma once
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>

#include <limits>
#include <algorithm>

#include "Vec2.h"

#ifndef HALF_CIRCLE
#define HALF_CIRCLE 180.0f
#endif

namespace lm
{
	template <typename T> class Vec3
	{
	private:
		T mX;
		T mY;
		T mZ;

	public:
		static const T mUnitVal;
		static const Vec3<T> mZero;
		static const Vec3<T> mOne;
		static const Vec3<T> mUp;
		static const Vec3<T> mDown;
		static const Vec3<T> mLeft;
		static const Vec3<T> mRight;
		static const Vec3<T> mForward;
		static const Vec3<T> mBackward;

		Vec3()
		{
			this->mX = 0;
			this->mY = 0;
			this->mZ = 0;
		}

		Vec3(const T pInit)
		{
			this->mX = pInit;
			this->mY = pInit;
			this->mZ = pInit;
		}

		Vec3(const T pX, const T pY, const T pZ)
		{
			this->mX = pX;
			this->mY = pY;
			this->mZ = pZ;
		}

		Vec3(const Vec3<T>& pOther)
		{
			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;
		}

		Vec3(Vec3<T>&& pOther) noexcept
		{
			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);
		}

		Vec3& operator=(const Vec3<T>& pOther)
		{
			if (this == &pOther)
				return *this;

			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;

			return *this;
		}

		Vec3& operator=(Vec3<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);

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

		const T Length() const
		{
			return sqrt((this->mX * this->mX) + (this->mY * this->mY) + (this->mZ * this->mZ));
		}

		const T Length2() const
		{
			return (this->mX * this->mX) + (this->mY * this->mY) + (this->mZ * this->mZ);
		}

		bool IsLongerThan(Vec3<T> const& pOther) const	// return true if this vector Length2 is greater than the other
		{
			return this->Length() > pOther.Length();
		}

		bool IsShorterThan(Vec3<T> const& pOther) const	// return true if this vector Length2 is less than the other
		{
			return this->Length() < pOther.Length();
		}

		const T DistanceFrom(Vec3<T> const& pOther) const // return distance between 2 points
		{
			return sqrt((pOther.mX - this.mX) * (pOther.mX - this.mX) +
				(pOther.mY - this.mY) * (pOther.mY - this.mY) +
				(pOther.mZ - this.mZ) * (pOther.mZ - this.mZ));
		}

		const bool IsUnit() const
		{
			return std::abs(this->Length() - Vec3<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - Vec3<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(Vec3<T>::mUnitVal));
		}

		const T DotProduct(const Vec3<T>& pOther) const
		{
			return (this->mX * pOther.mX) + (this->mY * pOther.mY) + (this->mZ * pOther.mZ);
		}

		const Vec3<T> CrossProduct(const Vec3<T>& pOther) const
		{
			return Vec3<T>((this->mY * pOther.mZ) - (this->mZ * pOther.mY),
				(this->mZ * pOther.mX) - (this->mX * pOther.mZ),
				(this->mX * pOther.mY) - (this->mY * pOther.mX));
		}

		const T RadAngle(const Vec3<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);

			const T lengthA2 = this->Length2();
			const T lengthB2 = pOther.Length2();
			const T length2Product = lengthA2 * lengthB2;

			const T length = sqrt(length2Product);

			return std::acos(product / length);
		}

		const T DegAngle(const Vec3<T>& pOther) const
		{
			return Utilities::RadiansToDegrees(this->RadAngle(pOther));
		}

		Vec3<T> ProjectOnTo(const Vec3<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);
			const T length = pOther.Length2();
			const T division = product / length;

			return Vec3<T>(division * pOther.mX, division * pOther.mY, division * pOther.mZ);
		}

		Vec3<T> Reflexion(const Vec3<T>& pOther) const
		{
			if (!pOther.IsUnit())
			{
				Vec3<T> proj = this->ProjectOnTo(pOther);
				proj *= 2;
				return Vec3<T>(this->mX - proj.mX, this->mY - proj.mY, this->mZ - proj.mZ);
			}

			const T product = this->DotProduct(pOther);
			const T doubledProduct = 2 * product;
			const Vec3<T> vecProduct = doubledProduct * pOther;

			return Vec3<T>(this->mX - vecProduct.mX, this->mY - vecProduct.mY,
				this->mZ - vecProduct.mZ);
		}

		Vec3<T> Perpendicular(const Vec3<T>& pOther) const
		{
			const Vec3<T> proj = this->ProjectOnTo(pOther);
			return Vec3<T>(this->mX - proj.mX, this->mY - proj.mY, this->mZ - proj.mZ);
		}

		Vec3<T> Add(const Vec3<T>& pOther) const
		{
			return Vec3<T>(this->mX + pOther.mX, this->mY + pOther.mY, this->mZ + pOther.mZ);
		}

		Vec3<T>& AddSelf(const Vec3<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			this->mZ += pOther.mZ;
			return *this;
		}

		Vec3<T> ScaleUniform(const T& pScale) const
		{
			return Vec3<T>(this->mX * pScale, this->mY * pScale, this->mZ * pScale);
		}

		Vec3<T> Scale(const Vec3<T>& pOther)
		{
			return lm::Vec3<T>(this->mX * pOther.mX, this->mY * pOther.mY, this->mZ * pOther.mZ);
		}

		Vec3<T>& ScaleSelf(const T pScale)
		{
			this->mX *= pScale;
			this->mY *= pScale;
			this->mZ *= pScale;
			return *this;
		}

		Vec3<T>& ScaleSelf(const Vec3<T>& pScale)
		{
			this->mX *= pScale.X();
			this->mY *= pScale.Y();
			this->mZ *= pScale.Z();
			return *this;
		}

		Vec3<T> Normalize() const
		{
			T length = this->Length();
			return Vec3<T>(this->mX / length, this->mY / length, this->mZ / length);
		}

		void NormalizeSelf()
		{
			T length = this->Length();
			this->mX /= length;
			this->mY /= length;
			this->mZ /= length;
		}

		static Vec3<T> Lerp(Vec3<T> Start, Vec3<T> End, float Alpha)
		{
			return Vec3<T>(((1 - Alpha) * Start) + (End * Alpha));
		}

		T& operator[](const int pIdx)
		{
			switch (pIdx)
			{
			case 0:		return mX;
			case 1:		return mY;
			case 2:		return mZ;

			case 'x':	return mX;
			case 'y':	return mY;
			case 'z':	return mZ;

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

			case 'x':	return mX;
			case 'y':	return mY;
			case 'z':	return mZ;

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

			default: return mX;
			}
		}

		const bool operator==(const Vec3<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			return std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(pOther.Length()));
		}

		const bool operator!=(const Vec3<T>& pOther) const
		{
			return !(*this == pOther);
		}

		const bool operator<(const Vec3<T>& pOther) const
		{
			return this->Length() < pOther.Length();
		}

		const bool operator<=(const Vec3<T>& pOther) const
		{
			return this->Length() < pOther.Length() || (*this == pOther);
		}

		const bool operator>(const Vec3<T>& pOther) const
		{
			return !(*this < pOther);
		}

		const bool operator>=(const Vec3<T>& pOther) const
		{
			return !(*this <= pOther);
		}

		Vec3<T> operator+(const Vec3<T>& pOther) const
		{
			return Vec3(this->mX + pOther.mX, this->mY + pOther.mY, this->mZ + pOther.mZ);
		}

		void operator+=(const Vec3<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			this->mZ += pOther.mZ;
		}
		
		void operator+=(const T& pValue)
		{
			this->mX += pValue;
			this->mY += pValue;
			this->mZ += pValue;
		}

		Vec3<T> operator-(const Vec3<T>& pOther) const
		{
			return Vec3(this->mX - pOther.mX, this->mY - pOther.mY, this->mZ - pOther.mZ);
		}

		void operator-=(const Vec3<T>& pOther)
		{
			this->mX -= pOther.mX;
			this->mY -= pOther.mY;
			this->mZ -= pOther.mZ;
		}

		void operator-=(const T& pValue)
		{
			this->mX -= pValue;
			this->mY -= pValue;
			this->mZ -= pValue;
		}

		Vec3<T> operator-() const
		{
			return Vec3(-this->mX, -this->mY, -this->mZ);
		}

		Vec3<T> operator*(const T pValue) const
		{
			return this->ScaleUniform(pValue);
		}

		void operator*=(const T pValue)
		{
			*this = this->ScaleUniform(pValue);
		}

		Vec3<T> operator*(const Vec3<T>& pOther) const
		{
			return Vec3<T>(this->mX * pOther.mX, this->mZ * pOther.mZ, this->mZ * pOther.mZ);
		}

		void operator*=(const Vec3<T>& pOther)
		{
			this->mX *= pOther.mX;
			this->mY *= pOther.mY;
			this->mZ *= pOther.mZ;
		}

		Vec3<T> operator/(const T pValue) const
		{
			return Vec3(this->mX / pValue, this->mY / pValue, this->mZ / pValue);
		}

		void operator/=(const T pValue)
		{
			this->mX /= pValue;
			this->mY /= pValue;
			this->mZ /= pValue;
		}

		Vec3<T> operator/(const Vec3<T>& pOther) const
		{
			return Vec3<T>(this->mX / pOther.mX, this->mY / pOther.mY, this->mZ / pOther.mZ);
		}

		void operator/=(const Vec3<T>& pOther)
		{
			this->mX /= pOther.mX;
			this->mY /= pOther.mY;
			this->mZ /= pOther.mZ;
		}

		void operator++(int)
		{
			T length = this->Length();
			this->mX *= (length + 1) / length;
			this->mY *= (length + 1) / length;
			this->mZ *= (length + 1) / length;
		}

		void operator--(int)
		{
			T length = this->Length();
			this->mX *= (length - 1) / length;
			this->mY *= (length - 1) / length;
			this->mZ *= (length - 1) / length;
		}
	};

	template<class T> Vec3<T> operator*(const double pValue, const Vec3<T>& pOther)
	{
		return pOther * T(pValue);
	}

	template<class T> Vec3<T> operator/(const double pValue, const Vec3<T>& pOther)
	{
		return Vec3<T>(T(pValue / pOther.X()), T(pValue / pOther.Y()), T(pValue / pOther.Z()));
	}

	template<class T> Vec3<T> operator/(const Vec3<T>& pFirst, const Vec3<T>& pSecond)
	{
		return Vec3<T>(pFirst.X() / pSecond.X(), pFirst.Y() / pSecond.Y(), pFirst.Z() / pSecond.Z());
	}

	template<class T> const T operator,(const Vec3<T>& pOther1, const Vec3<T>& pOther2)
	{
		return pOther1.DotProduct(pOther2);
	}

	template<class T> const Vec3<T> Vec3<T>::mZero = Vec3();
	template<class T> const Vec3<T> Vec3<T>::mOne = Vec3(1);
	template<class T> const Vec3<T> Vec3<T>::mUp(0, 1, 0);
	template<class T> const Vec3<T> Vec3<T>::mDown(0, -1, 0);
	template<class T> const Vec3<T> Vec3<T>::mLeft(-1, 0, 0);
	template<class T> const Vec3<T> Vec3<T>::mRight(1, 0, 0);
	template<class T> const Vec3<T> Vec3<T>::mForward(0, 0, 1);
	template<class T> const Vec3<T> Vec3<T>::mBackward(0, 0, -1);
	template<class T> const T Vec3<T>::mUnitVal = 1;

	typedef Vec3<float>		FVec3;
	typedef Vec3<double>	DVec3;
}