#pragma once

#include <fstream>
#include <string>

#include "Config.h"
#include "Math/Util.h"

namespace FusionCrowd
{
	class FUSION_CROWD_API NavMeshPoly
	{
	public:
		NavMeshPoly();
		~NavMeshPoly();

		NavMeshPoly& operator=(const NavMeshPoly& n);
		bool containsPoint(const DirectX::SimpleMath::Vector2& point) const;
		float getElevation(const DirectX::SimpleMath::Vector2& point) const;
		DirectX::SimpleMath::Vector2 getGradient() const { return DirectX::SimpleMath::Vector2(A, B); }
		bool loadFromAscii(std::ifstream& f);
		bool loadFromBinary(std::ifstream& f);
		void initialize(size_t vCount, unsigned int* ids, float A = 0.f, float B = 0.f,
		                float C = 0.f);

		unsigned int* vertIDs;
		size_t vertCount;
		const DirectX::SimpleMath::Vector2* vertices;
		float minX;
		float maxX;
		float minY;
		float maxY;
		void setBB(const DirectX::SimpleMath::Vector2* vertices);
		float A;
		float B;
		float C;
	};
}