#pragma once

#include "TestCases/ITestCase.h"

#include <vector>

namespace TestFusionCrowd
{
	class CorridorTestCase : public ITestCase
	{
	public:
		CorridorTestCase(size_t halfNum, float groupFraction, size_t steps);

		void Pre() override;
		std::string GetName() const override { return "Corridor"; };

	private:
		FusionCrowd::ComponentId _opComponent;
		size_t _groups;
		size_t _singles;

		std::vector<std::vector<bool>> _busy;

		void SetUpGroup(float gx, float gy);
		void SetUpSingle(float x1, float x2, float y1, float y2);
	};
}
