#pragma once

#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef HALF_CIRCLE
#define HALF_CIRCLE 180.0f
#endif

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


namespace lm
{
	namespace Utilities
	{
		inline const double RadiansToDegrees(const double pRad)
		{
			return pRad * (HALF_CIRCLE / M_PI);
		}

		inline const double DegreesToRadians(const double pDeg)
		{
			return pDeg * (M_PI / HALF_CIRCLE);
		}

		inline float FClamp(const float pValue, const float pLower, const float pUpper)
		{
			return std::max(pLower, std::min(pValue, pUpper));
		}

		inline float FLerp(const float pMin, const float pMax, const float pValue)
		{
			return pMin + pValue * (pMax - pMin);
		}

		inline float FUnLerp(const float pMin, const float pMax, const float pValue)
		{
			return (pValue - pMin) / (pMax - pMin);
		}

		inline float MapInRange(const float pValue, const float pInMin, const float pInMax,
			const float pOutMin, const float pOutMax)
		{
			return (pValue - pInMin) / (pInMax - pInMin) * (pOutMax - pOutMin) + pOutMin;
		}
	};
}