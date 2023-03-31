#pragma once

#include "Vec3.h"
namespace lm
{
	class Vertex
	{
		FVec3 mPosition{};
		FVec3 mNormal{};
		FVec2 mTextCoords{};
		FVec3 mColor{};
	public:

		Vertex()
	: mPosition{ 0.0f, 0.0f, 0.0f }, mNormal{ 0.0f, 0.0f, 0.0f }
	{}
		Vertex(const float pX, const float pY, const float pZ, const float pXNormal = 0.0f,
			const float pYNormal = 0.0f, const float pZNormal = 0.0f,
			const float pTextCoordX = 0.0f, const float pTextCoordY = 0.0f)
	: mPosition{ pX, pY, pZ }, mNormal{ pXNormal, pYNormal, pZNormal },
	mTextCoords{ pTextCoordX, pTextCoordY }
	{}

		Vertex(const Vertex& pOther) = default;
		explicit Vertex(const FVec3& pPosition, const FVec3& pNormal = FVec3{},
			const FVec2& pTextCoord = FVec2{}, const FVec3& pColor = FVec3{})
	: mPosition{ pPosition }, mNormal{ pNormal }, mTextCoords{ pTextCoord }, mColor{ pColor }
	{}


		float DotProduct(const Vertex& pOther)
		{
			return mPosition.X() * pOther.mPosition.X() + mPosition.Y() * pOther.mPosition.Y() +
				mPosition.Z() * pOther.mPosition.Z();
		}


		Vertex	operator-(Vertex& pVert)
		{
			return Vertex(this->mPosition.X() - pVert.mPosition.X(),
				this->mPosition.Y() - pVert.mPosition.Y(), this->mPosition.Z() - pVert.mPosition.Z());
		}

		bool	operator==(const Vertex& pOther) const
		{
			return mPosition == pOther.mPosition && mColor == pOther.mColor &&
				mTextCoords == pOther.mTextCoords;
		}

		FVec3& GetPosition() { return mPosition; }
		FVec3& GetNormal() { return mNormal; }
		FVec2& GetTextCoords() { return mTextCoords; }
		FVec3& GetColor() { return mColor; }
	};
}