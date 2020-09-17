#pragma once

#include "TestCases/ITestCase.h"
#include "Export/ComponentId.h"

namespace TestFusionCrowd
{
	class TradeshowTestCase : public ITestCase
	{
	public:
		TradeshowTestCase(size_t agentsNum, size_t simSteps, bool writeTraj);

		TradeshowTestCase(
			size_t agentsNum,
			size_t simSteps,
			FusionCrowd::ComponentId opComponent,
			size_t maxRewinds,
			std::string customName
		);

		void Pre() override;
		std::string GetName() const override { return _name; };

	private:
		size_t _maxRewinds = 0;
		std::string _name = "Tradeshow";
		FusionCrowd::ComponentId _op = FusionCrowd::ComponentIds::ORCA_ID;
	};
}
