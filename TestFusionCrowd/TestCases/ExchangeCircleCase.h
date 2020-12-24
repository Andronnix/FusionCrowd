#pragma once

#include "TestCases/ITestCase.h"

#include "Export/ComponentId.h"

namespace TestFusionCrowd
{
	class ExchangeCircleCase : public ITestCase
	{
	public:
		ExchangeCircleCase(size_t agentsNum, size_t steps, FusionCrowd::ComponentId op, bool writeTraj);
		ExchangeCircleCase(
			size_t agentsNum,
			size_t steps,
			FusionCrowd::ComponentId op,
			size_t maxRewinds,
			std::string customName
		);

		void Pre() override;
		std::string GetName() const override { return _name; };
	private:
		FusionCrowd::ComponentId _op;
		size_t _rewinds = 0;
		std::string _name = "ExchangeCircleCase";
	};
}
