#pragma once
#include <cmath>
#include <algorithm>
#include "Mat3.h"
#include "Mat4.h"

	namespace lm
{
	template <typename T> class Quaternion
	{
	public:
		T mW, mX, mY, mZ;

		static const T mUnitVal;
		static const Quaternion<T> mZero;
		static const Quaternion<T> mOne;

		Quaternion()
		{
			mW = 0;
			mX = 0;
			mY = 0;
			mZ = 0;
		}

		Quaternion(const T pValue)
			:mW(pValue), mX(pValue), mY(pValue), mZ(pValue)
		{}

		Quaternion(const T pX, const T pY, const T pZ, const T pW)
			:mW(pW), mX(pX), mY(pY), mZ(pZ)
		{}


		/* Rule of five */
		Quaternion(const Quaternion<T>& pOther)
		{
			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;
			this->mW = pOther.mW;
		}

		Quaternion<T>& operator=(const Quaternion& pOther)
		{
			if (this == &pOther)
				return *this;

			this->mX = pOther.mX;
			this->mY = pOther.mY;
			this->mZ = pOther.mZ;
			this->mW = pOther.mW;

			return *this;
		}

		Quaternion(Quaternion<T>&& pOther) noexcept
		{
			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);
			this->mW = std::move(pOther.mW);
		}

		Quaternion<T>& operator=(Quaternion<T>&& pOther) noexcept
		{
			if (this == &pOther)
				return *this;

			this->mX = std::move(pOther.mX);
			this->mY = std::move(pOther.mY);
			this->mZ = std::move(pOther.mZ);
			this->mW = std::move(pOther.mW);

			return *this;
		}

		/**/

		Quaternion<T> operator+(const Quaternion<T>& pOther) const
		{
			return Quaternion<T>(this->mX + pOther.mX, this->mY + pOther.mY, this->mZ + pOther.mZ,
				this->mW + pOther.mW);
		}
		void operator+=(const Quaternion<T>& pOther) /*const*/
		{
			this->mW += pOther.mW;
			this->mX += pOther.mX;
			this->mY += pOther.mY;
			this->mZ += pOther.mZ;
		}
		Quaternion<T> operator-(const Quaternion<T>& pOther) const
		{
			return Quaternion<T>(this->mX - pOther.mX, this->mY - pOther.mY, this->mZ - pOther.mZ,
				this->mW - pOther.mW);
		}
		void operator-=(const Quaternion<T>& pOther)
		{
			this->mW -= pOther.mW;
			this->mX -= pOther.mX;
			this->mY -= pOther.mY;
			this->mZ -= pOther.mZ;
		}
		Quaternion<T> operator*(const T& pScalaire) const
		{
			return Quaternion<T>(this->mX * pScalaire, this->mY * pScalaire, this->mZ * pScalaire,
				this->mW * pScalaire);
		}
		void operator*=(const T& pScalaire)
		{
			this->mW *= pScalaire;
			this->mX *= pScalaire;
			this->mY *= pScalaire;
			this->mZ *= pScalaire;
		}
		Quaternion<T> operator*(const Quaternion<T>& pOther) const
		{
			T FinalW = (this->mW * pOther.mW) - (this->mX * pOther.mX) - (this->mY * pOther.mY) - (this->mZ * pOther.mZ);
			T FinalX = (this->mW * pOther.mX) + (this->mX * pOther.mW) + (this->mY * pOther.mZ) - (this->mZ * pOther.mY);
			T FinalY = (this->mW * pOther.mY) - (this->mX * pOther.mZ) + (this->mY * pOther.mW) + (this->mZ * pOther.mX);
			T FinalZ = (this->mW * pOther.mZ) + (this->mX * pOther.mY) - (this->mY * pOther.mX) + (this->mZ * pOther.mW);

			return Quaternion<T>(FinalX, FinalY, FinalZ, FinalW);
		}

		void operator*=(const Quaternion<T>& pOther)
		{
			T FinalW = (this->mW * pOther.mW) - (this->mX * pOther.mX) - (this->mY * pOther.mY) - (this->mZ * pOther.mZ);
			T FinalX = (this->mW * pOther.mX) + (this->mX * pOther.mW) + (this->mY * pOther.mZ) - (this->mZ * pOther.mY);
			T FinalY = (this->mW * pOther.mY) - (this->mX * pOther.mZ) + (this->mY * pOther.mW) + (this->mZ * pOther.mX);
			T FinalZ = (this->mW * pOther.mZ) + (this->mX * pOther.mY) - (this->mY * pOther.mX) + (this->mZ * pOther.mW);

			this->mW = FinalW;
			this->mX = FinalX;
			this->mY = FinalY;
			this->mZ = FinalZ;
		}

		lm::Vec3<T> operator*(const lm::Vec3<T> pOther)
		{
			Quaternion<T> inverseQuat = this->Conjugate();
			Quaternion<T> VecToQuat = Quaternion<T>(pOther.X(), pOther.Y(), pOther.Z(), 0);
			Quaternion<T> newQuat = *this * VecToQuat;
			newQuat *= inverseQuat;
			return lm::Vec3<T>(newQuat.mX, newQuat.mY, newQuat.mZ);
		}

		const bool operator==(const Quaternion<T>& pOther) const
		{
			if (this == &pOther)
				return true;

			if (this->mW == pOther.mW && this->mX == pOther.mX && this->mY == pOther.mY &&
				this->mZ == pOther.mZ)
				return true;

			return false;
		}
		const bool operator!=(const Quaternion<T>& pOther) const
		{
			return !(*this == pOther);
		}

		Quaternion<T> Conjugate() const
		{
			return Quaternion<T>(this->mX * -1, this->mY * -1, this->mZ * -1, this->mW);
		}

		T Length() const
		{
			return sqrt(this->mW * this->mW +
				this->mX * this->mX +
				this->mY * this->mY + 
				this->mZ * this->mZ);
		}

		Quaternion<T> Normalize() const
		{
			auto length = this->Length();

			return Quaternion<T>(this->mX / length,
				this->mY / length,
				this->mZ / length,
				this->mW / length);
		}

		void NormalizeSelf()
		{
			auto length = this->Length();

			this->mX /= length;
			this->mY /= length;
			this->mZ /= length;
			this->mW /= length;
		}

		const bool IsUnit() const
		{
			return std::abs(this->Length() - Vec4<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() ||
				std::abs(this->Length() - Vec4<T>::mUnitVal) <= std::numeric_limits<T>::epsilon() *
				std::max(std::abs(this->Length()), std::abs(Vec4<T>::mUnitVal));
		}

		static Quaternion<T> GetRotationQuaternion(const Mat3<T>& pMat3)
		{
			Quaternion<T> result;

			float m00 = pMat3[0][0];
			float m11 = pMat3[1][1];
			float m22 = pMat3[2][2];

			float sum = m00 + m11 + m22;

			if (sum > 0.0f)
			{
				result.mW = sqrt(sum + 1.0f) * 0.5f;
				float f = 0.25f / result.mW;

				result.mX = (pMat3[2][1] - pMat3[1][2]) * f;
				result.mY = (pMat3[0][2] - pMat3[2][0]) * f;
				result.mZ = (pMat3[1][0] - pMat3[0][1]) * f;
			}
			else if ((m00 > m11) && (m00 > m22))
			{
				result.mX = sqrt(m00 - m11 - m22 + 1.0f) * 0.5f;
				float f = 0.25f / result.mX;

				result.mY = (pMat3[1][0] + pMat3[0][1]) * f;
				result.mZ = (pMat3[0][2] + pMat3[2][0]) * f;
				result.mW = (pMat3[2][1] - pMat3[1][2]) * f;
			}
			else if (m11 > m22)
			{
				result.mY = sqrt(m11 - m00 - m22 + 1.0f) * 0.5f;
				float f = 0.25f / result.mY;

				result.mX = (pMat3[1][0] + pMat3[0][1]) * f;
				result.mZ = (pMat3[2][1] + pMat3[1][2]) * f;
				result.mW = (pMat3[0][2] - pMat3[2][0]) * f;
			}
			else
			{
				result.mZ = sqrt(m22 - m00 - m11 + 1.0f) * 0.5f;
				float f = 0.25f / result.mZ;

				result.mX = (pMat3[0][2] + pMat3[2][0]) * f;
				result.mY = (pMat3[2][1] + pMat3[2][1]) * f;
				result.mW = (pMat3[1][0] - pMat3[0][1]) * f;
			}

			return result.Conjugate().Normalize();
		}

		Mat3<T> GetRotationMatrix() const
		{
			Quaternion<T> base = *this;

			float x2 = base.mX * base.mX;
			float y2 = base.mY * base.mY;
			float z2 = base.mZ * base.mZ;

			float xy = base.mX * base.mY;
			float xz = base.mX * base.mZ;
			float yz = base.mY * base.mZ;
			float wx = base.mW * base.mX;
			float wy = base.mW * base.mY;
			float wz = base.mW * base.mZ;

			float m00 = 1 - 2 * y2 - 2 * z2;
			float m01 = 2 * (xy - wz);
			float m02 = 2 * (xz + wy);
			float m10 = 2 * (xy + wz);
			float m11 = 1 - 2 * x2 - 2 * z2;
			float m12 = 2 * (yz - wx);
			float m20 = 2 * (xz - wy);
			float m21 = 2 * (yz + wx);
			float m22 = 1 - 2 * x2 - 2 * y2;

			Vec3<T> l1(m00, m01, m02);
			Vec3<T> l2(m10, m11, m12);
			Vec3<T> l3(m20, m21, m22);

			return Mat3<T>(l1, l2, l3).Transpose();
		}

		Mat4<T> ToMat4() const
		{
			lm::Quaternion<T> base = this->Normalize();
			lm::Mat4<T> mat4;

			mat4[0][0] = 1 - (2 * (base.mY * base.mY)) - (2 * (base.mZ * base.mZ));
			mat4[1][0] = 2 * ((base.mX * base.mY) - (base.mZ * base.mW));
			mat4[2][0] = 2 * ((base.mX * base.mZ) + (base.mW * base.mY));
			mat4[3][0] = 0;

			mat4[0][1] = 2 * ((base.mX * base.mY) + (base.mW * base.mZ));
			mat4[1][1] = 1 - (2 * (base.mX * base.mX)) - (2 * (base.mZ * base.mZ));
			mat4[2][1] = 2 * ((base.mY * base.mZ) - (base.mW * base.mX));
			mat4[3][1] = 0;

			mat4[0][2] = 2 * ((base.mX * base.mZ) - (base.mW * base.mY));
			mat4[1][2] = 2 * ((base.mY * base.mZ) + (base.mW * base.mX));
			mat4[2][2] = 1 - (2 * (base.mX * base.mX)) - (2 * (base.mY * base.mY));
			mat4[3][2] = 0;

			mat4[0][3] = 0;
			mat4[1][3] = 0;
			mat4[2][3] = 0;
			mat4[3][3] = 1;

			return mat4;
		}

		static Quaternion<T> RotationAxis(float pAngle, Vec3<T> pDirection)
		{
			//step 1: is unit vector ?
			Vec3<T> UnitVector = pDirection.Normalize();

			//step 2 & 3
			Quaternion<T> g = Quaternion<T>(sin(pAngle / 2) * UnitVector.X(),
				sin(pAngle / 2) * UnitVector.Y(),
				sin(pAngle / 2) * UnitVector.Z(),
				cos(pAngle / 2));

			return g;
		}

		enum class axis_order
		{
			XYZ, XZY, YXZ, YZX, ZXY, ZYX
		};

		/* --- DirectX way --- */
		static Quaternion<T> EulerToQuat(float pRoll, float pPitch, float pYaw, axis_order pOrder)
		{
			Quaternion<T> xRot = RotationAxis(pRoll, Vec3<T>::mLeft);
			Quaternion<T> yRot = RotationAxis(-pPitch, Vec3<T>::mUp);
			Quaternion<T> zRot = RotationAxis(-pYaw, Vec3<T>::mForward);

			switch (pOrder)
			{
				case axis_order::XYZ: return (xRot * yRot * zRot).Conjugate();
				case axis_order::XZY: return (xRot * zRot * yRot).Conjugate();
				case axis_order::YXZ: return (yRot * xRot * zRot).Conjugate();
				case axis_order::YZX: return (yRot * zRot * xRot).Conjugate();
				case axis_order::ZXY: return (zRot * xRot * yRot).Conjugate();
				case axis_order::ZYX: return (zRot * yRot * xRot).Conjugate();

				default:
					return Quaternion<T>();
			}
		}
		
		static Quaternion<T> EulerToQuat(const Vec3<T>& pAngles , axis_order pOrder)
		{
			auto xRot = RotationAxis(pAngles.X(), Vec3<T>::mLeft);
			auto yRot = RotationAxis(-pAngles.Y(), Vec3<T>::mUp);
			auto zRot = RotationAxis(-pAngles.Z(), Vec3<T>::mForward);

			switch (pOrder)
			{
			case axis_order::XYZ: return (xRot * yRot * zRot).Conjugate();
			case axis_order::XZY: return (xRot * zRot * yRot).Conjugate();
			case axis_order::YXZ: return (yRot * xRot * zRot).Conjugate();
			case axis_order::YZX: return (yRot * zRot * xRot).Conjugate();
			case axis_order::ZXY: return (zRot * xRot * yRot).Conjugate();
			case axis_order::ZYX: return (zRot * yRot * xRot).Conjugate();

			default:
				return nullptr;
			}
		}

		void QuatToEuler321Sequence(float& pRoll, float& pPitch, float& pYaw) const
		{
			Quaternion<T> normalizedQuat = this->Normalize();

			auto test = normalizedQuat.mX * normalizedQuat.mY + normalizedQuat.mZ * normalizedQuat.mW;

			if (test > 0.499) { // singularity at North Pole
				pPitch = 2 * atan2(normalizedQuat.mX, normalizedQuat.mW);
				pYaw = M_PI / 2;
				pRoll = 0;
				return;
			}
			if (test < -0.499) { // singularity at South Pole
				pPitch = -2 * atan2(normalizedQuat.mX, normalizedQuat.mW);
				pYaw = -M_PI / 2;
				pRoll = 0;
				return;
			}

			// roll (x-axis rotation)
			double sinr_cosp = 2 * (normalizedQuat->mW * normalizedQuat->mX +
				normalizedQuat->mY * normalizedQuat->mZ);
			double cosr_cosp = 1 - 2 * (normalizedQuat->mX * normalizedQuat->mX +
				normalizedQuat->mY * normalizedQuat->mY);
			pRoll = std::atan2(sinr_cosp, cosr_cosp);

			// pitch (y-axis rotation)
			double sinp = std::sqrt(1 + 2 * (normalizedQuat->mW * normalizedQuat->mY -
				normalizedQuat->mX * normalizedQuat->mZ));
			double cosp = std::sqrt(1 - 2 * (normalizedQuat->mW * normalizedQuat->mY -
				normalizedQuat->mX * normalizedQuat->mZ));
			pPitch = 2 * std::atan2(sinp, cosp) - M_PI / 2;

			// yaw (z-axis rotation)
			double siny_cosp = 2 * (normalizedQuat->mW * normalizedQuat->mZ +
				normalizedQuat->mX * normalizedQuat->mY);
			double cosy_cosp = 1 - 2 * (normalizedQuat->mY * normalizedQuat->mY +
				normalizedQuat->mZ * normalizedQuat->mZ);
			pYaw = std::atan2(siny_cosp, cosy_cosp);
		}

		const T DotProduct(const Quaternion<T>& pOther) const
		{
			return (this->mX * pOther.mX) +
				(this->mY * pOther.mY) +
				(this->mZ * pOther.mZ) +
				(this->mW * pOther.mW);
		}

		static Quaternion<T> Lerp(const Quaternion<T>& pStart, const Quaternion<T>& pEnd, float pAlpha)
		{
			float t = Utilities::FClamp(pAlpha, 0.f, 1.f);
			Quaternion<T> q = (pEnd - pStart);
			q *= t;
			q += pStart;

			return q;
		}

		static Quaternion<T> Slerp(const Quaternion<T>& pStart, 
			const Quaternion<T>& pEnd,
			float pAlpha)
		{
			Quaternion<T> from = pStart;
			Quaternion<T> to = pEnd;

			float t = Utilities::FClamp(pAlpha, 0.f, 1.f);
			Quaternion<T> q;
			float cosAngle = from.DotProduct(to);

			if (cosAngle < 0.f)
			{
				cosAngle = -cosAngle;
				to = Quaternion<T>(-to.mX, -to.mY, -to.mZ, -to.mW);
			}

			if (cosAngle < 0.95f)
			{
				float angle = std::acos(cosAngle);
				float sinAngle = std::sin(angle);
				float invSinAngle = 1.f / sinAngle;

				float ratioA = std::sin((1 - t) * angle) * invSinAngle;
				float ratioB = std::sin(t * angle) * invSinAngle;

				return Quaternion<T>(from.mX * ratioA + to.mX * ratioB,
					from.mY * ratioA + to.mY * ratioB,
					from.mZ * ratioA + to.mZ * ratioB,
					from.mW * ratioA + to.mW * ratioB);
			}
			else
			{
				return Quaternion::Lerp(pStart, pEnd, pAlpha);
			}
		}
	};

	template<class T> const Quaternion<T> Quaternion<T>::mZero = Quaternion();
	template<class T> const Quaternion<T> Quaternion<T>::mOne = Quaternion(1);
	template<class T> const T Quaternion<T>::mUnitVal = 1;

	typedef Quaternion<float> FQuat;
	typedef Quaternion<double> DQuat;
}