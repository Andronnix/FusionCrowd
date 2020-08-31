#pragma once

#include "TestCases/ITestCase.h"
#include "Export/ComponentId.h"
#include "Export/Export.h"

#include <memory>
#include <vector>
#include <chrono>

namespace TestFusionCrowd
{
	class DebugTestCase : public ITestCase
	{
	public:
		DebugTestCase(
			FusionCrowd::ComponentId opComponent=FusionCrowd::ComponentIds::KARAMOUZAS_ID,
			size_t simulationSteps=5000
		);

		void Pre() override;
		std::string GetName() const override { return "Debug"; };

	private:
		FusionCrowd::ComponentId _opComponent;
	};
}
