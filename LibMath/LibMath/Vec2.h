#pragma once

#define _USE_MATH_DEFINES_
#include <cmath>
#include "Utilities.h"

#include <iostream>
#include <string>

#ifdef max
#undef max
#endif // max

namespace lm
{
	template <typename T> class Vec2
	{
	private:
		T mX;
		T mY;

	public:
		static const T mUnitVal;
		static const Vec2<T> mZero;
		static const Vec2<T> mOne;
		static const Vec2<T> mUp;
		static const Vec2<T> mDown;
		static const Vec2<T> mLeft;
		static const Vec2<T> mRight;

		Vec2()
		{
			this->mX = 0;
			this->mY = 0;
		}

		Vec2(const T init)
		{
			this->mX = init;
			this->mY = init;
		}

		Vec2(const T pX, const T pY)
		{
			this->mX = pX;
			this->mY = pY;
		}

		Vec2(const Vec2<T>& pOther)
		{
			this->mX = pOther.mX;
			this->mY = pOther.mY;
		}

		Vec2(Vec2<T>&& pOther) noexcept
		{
			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
		}

		Vec2<T>& operator=(const Vec2<T>& pOther)
		{
			if (this == &pOther)
				return *this;

			this->mX = pOther.mX;
			this->mY = pOther.mY;

			return *this;
		}

		Vec2<T>& operator=(Vec2<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);

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

		T X() const
		{
			return this->mX;
		}

		T Y() const
		{
			return this->mY;
		}

		const T Length() const
		{
			return sqrt((this->mX * this->mX) + (this->mY * this->mY));
		}

		const T Length2() const
		{
			return (this->mX * this->mX) + (this->mY * this->mY);
		}

		bool IsLongerThan(Vec2<T> const& pOther) const	// return true if this vector Length2 is greater than the other
		{
			return this->Length() > pOther.Length();
		}

		bool IsShorterThan(Vec2<T> const& pOther) const	// return true if this vector Length2 is less than the other
		{
			return this->Length() < pOther.Length();
		}

		const T DistanceFrom(Vec2<T> const& pOther) const // return distance between 2 points
		{
			return sqrt((pOther.mX - this.mX) * (pOther.mX - this.mX) +
				(pOther.mY - this.mY) * (pOther.mY - this.mY));
		}

		const bool IsUnit() const
		{
			return std::abs(this->Length() - Vec2<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - Vec2<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(Vec2<T>::mUnitVal));
		}

		const T DotProduct(const Vec2<T>& pOther) const
		{
			return (this->mX * pOther.mX) + (this->mY * pOther.mY);
		}

		const T CrossProduct(const Vec2<T>& pOther) const
		{
			return (this->mX* pOther.mY) - (this->mY * pOther.mX);
		}

		const T RadAngle(const Vec2<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);

			const T lengthA2 = this->Length2();
			const T lengthB2 = pOther.Length2();
			const T length2Product = lengthA2 * lengthB2;

			const T length = sqrt(length2Product);

			return std::acos(product / length);
		}

		const T DegAngle(const Vec2<T>& pOther) const
		{
			return(const T)Utilities::RadiansToDegrees(this->RadAngle(pOther));
		}

		Vec2<T> ProjectOnTo(const Vec2<T>& pOther) const
		{
			const T product = this->DotProduct(pOther);
			const T length = pOther.Length2();
			const T division = product / length;

			return Vec2<T>(division * pOther.mX, division * pOther.mY);
		}

		Vec2<T> Reflexion(const Vec2<T>& pOther) const
		{
			if (!pOther.IsUnit())
			{
				Vec2<T> proj = this->ProjectOnTo(pOther);
				proj *= 2;
				return Vec2<T>(this->mX - proj.mX, this->mY - proj.mY);
			}

			const T product = this->DotProduct(pOther);
			const T doubledProduct = 2 * product;
			const Vec2<T> vecProduct = doubledProduct * pOther;

			return Vec2<T>(this->mX - vecProduct.mX, this->mY - vecProduct.mY);
		}

		Vec2<T> Perpendicular(const Vec2<T>& pOther) const
		{
			const Vec2<T> proj = this->ProjectOnTo(pOther);
			return Vec2<T>(this->mX - proj.mX, this->mY - proj.mY);
		}

		Vec2<T> Add(const Vec2<T>& pOther) const
		{
			return Vec2<T>(this->mX + pOther.mX, this->mY + pOther.mY);
		}

		Vec2<T>& AddSelf(const Vec2<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			return *this;
		}

		Vec2<T> Scale(const T pScale) const
		{
			return Vec2<T>(this->mX * pScale, this->mY * pScale);
		}

		Vec2<T>& ScaleSelf(const T pScale)
		{
			this->mX *= pScale;
			this->mY *= pScale;
			return *this;
		}

		Vec2<T> Normalize() const
		{
			return Vec2<T>(this->mX / this->Length(), this->mY / this->Length());
		}

		void NormalizeSelf()
		{
			T length = this->Length();
			this->mX /= length;
			this->mY /= length;
		}

		T& operator[](const int pIdx)
		{
			switch (pIdx)
			{
			case 0:		return mX;
			case 1:		return mY;

			case 'x':	return mX;
			case 'y':	return mY;

			default:	return mX;
			}
		}

		T operator[](const int pIdx) const
		{
			switch (pIdx)
			{
			case 0:		return mX;
			case 1:		return mY;

			case 'x':	return mX;
			case 'y':	return mY;

			default:	return mX;
			}
		}

		T& operator[](const char* pIdx)
		{
			switch (pIdx[0])
			{
			case 'x': return mX;
			case 'y': return mY;

			default: return mX;
			}
		}

		const T operator[](const char* pIdx) const
		{
			switch (pIdx[0])
			{
			case 'x': return mX;
			case 'y': return mY;

			default: return mX;
			}
		}

		const bool operator==(const Vec2<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			return std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - pOther.Length()) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(pOther.Length()));
		}

		const bool operator!=(const Vec2<T>& pOther) const
		{
			return !(*this == pOther);
		}

		const bool operator<(const Vec2<T>& pOther) const
		{
			return this->Length() < pOther.Length();
		}

		const bool operator<=(const Vec2<T>& pOther) const
		{
			return this->Length() < pOther.Length() || (*this == pOther);
		}

		const bool operator>(const Vec2<T>& pOther) const
		{
			return !(*this < pOther);
		}

		const bool operator>=(const Vec2<T>& pOther) const
		{
			return !(*this <= pOther);
		}

		Vec2<T> operator+(const Vec2<T>& pOther) const
		{
			return Vec2(this->mX + pOther.mX, this->mY + pOther.mY);
		}

		void operator+=(const Vec2<T>& pOther)
		{
			this->mX += pOther.mX;
			this->mY += pOther.mY;
		}

		Vec2<T> operator-(const Vec2<T>& pOther) const
		{
			return Vec2(this->mX - pOther.mX, this->mY - pOther.mY);
		}

		void operator-=(const Vec2<T>& pOther)
		{
			this->mX -= pOther.mX;
			this->mY -= pOther.mY;
		}

		Vec2<T> operator-() const
		{
			return Vec2(-this->mX, -this->mY);
		}

		Vec2<T> operator*(const T pValue) const
		{
			return this->Scale(pValue);
		}

		void operator*=(const T pValue)
		{
			*this = this->Scale(pValue);
		}

		Vec2<T> operator*(const Vec2<T>& pOther) const
		{
			return Vec2<T>(this->mX * pOther.mX, this->mY * pOther.mY);
		}

		void operator*=(const Vec2<T>& pOther)
		{
			this->mX *= pOther.mX;
			this->mY *= pOther.mY;
		}

		Vec2<T> operator/(const T pValue) const
		{
			return Vec2(this->mX / pValue, this->mY / pValue);
		}

		void operator/=(const T pValue)
		{
			this->mX /= pValue;
			this->mY /= pValue;
		}

		Vec2<T> operator/(const Vec2<T>& pOther) const
		{
			return Vec2<T>(this->mX / pOther.mX, this->mY / pOther.mY);
		}

		void operator/=(const Vec2<T>& pOther)
		{
			this->mX /= pOther.mX;
			this->mY /= pOther.mY;
		}

		void operator++(int)
		{
			T length = this->Length();
			this->mX *= (length + 1) / length;
			this->mY *= (length + 1) / length;
		}

		void operator--(int)
		{
			T length = this->Length();
			this->mX *= (length - 1) / length;
			this->mY *= (length - 1) / length;
		}
	};

	template<class T> std::ostream& operator<<(std::ostream& pOs, const Vec2<T>& pOther)
	{
		return pOs << pOther.X() << ',' << pOther.Y();
	}

	template<class T> void operator>>(std::istream& pIs, Vec2<T>& pOther)
	{
		std::string str;
		pIs >> str;

		int delimiter = -1;
		for (unsigned int i = 0; i < str.length(); i++)
			if (str[i] == ',') {
				delimiter = i;
				break;
			}

		if (delimiter == -1 || (str[0] < '0' || str[0] > '9')
			|| (str[delimiter + 1] < '0' || str[delimiter + 1] > '9'))
		{
			pOther.X() = 0;
			pOther.Y() = 0;
		}
		else
		{
			std::string xStr = str.substr(0, delimiter);
			std::string yStr = str.substr(++delimiter, str.length());

			pOther.X() = std::stof(xStr);
			pOther.Y() = std::stof(yStr);
		}
	}

	template<class T> std::string operator+(const std::string& pStr, const Vec2<T>& pOther)
	{
		return  pStr + "x:" + std::to_string(pOther.X()) + ", y : " + std::to_string(pOther.Y());
	}

	template<class T> void operator+=(std::string& pStr, const Vec2<T>& pOther)
	{
		pStr += "x:" + std::to_string(pOther.X()) + ", y : " + std::to_string(pOther.Y());
	}

	template<class T> Vec2<T> operator*(const double pValue, const Vec2<T>& pOther)
	{
		return pOther * T(pValue);
	}

	template<class T> Vec2<T> operator/(const double pValue, const Vec2<T>& pOther)
	{
		return Vec2<T>(T(pValue / pOther.X()), T(pValue / pOther.Y()));
	}

	template<class T> const T operator,(const Vec2<T>& pOther1, const Vec2<T>& pOther2)
	{
		return pOther1.DotProduct(pOther2);
	}

	template<class T> const Vec2<T> Vec2<T>::mZero = Vec2();
	template<class T> const Vec2<T> Vec2<T>::mOne = Vec2(1);
	template<class T> const Vec2<T> Vec2<T>::mUp(0, 1);
	template<class T> const Vec2<T> Vec2<T>::mDown(0, -1);
	template<class T> const Vec2<T> Vec2<T>::mLeft(-1, 0);
	template<class T> const Vec2<T> Vec2<T>::mRight(1, 0);
	template<class T> const T Vec2<T>::mUnitVal = 1;

	typedef Vec2<float> FVec2;
	typedef Vec2<double> DVec2;
}