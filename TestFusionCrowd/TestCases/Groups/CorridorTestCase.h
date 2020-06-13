#pragma once

#include "TestCases/ITestCase.h"

namespace TestFusionCrowd
{
	class CorridorTestCase : public ITestCase
	{
	public:
		CorridorTestCase(size_t groupsNum, size_t steps);

		void Pre() override;
		std::string GetName() const override { return "Corridor"; };

	private:
		FusionCrowd::ComponentId _opComponent;
		size_t _groupsNum;

		void SetUpGroup(float gx, float gy);
	};
}
