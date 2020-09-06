#include "pch.h"

#include "DebugTestCase.h"

using namespace FusionCrowd;

namespace TestFusionCrowd
{
	DebugTestCase::DebugTestCase(FusionCrowd::ComponentId opComponent, size_t simulationSteps):
		ITestCase(2, simulationSteps, true),
		_opComponent(opComponent)
	{
	}

	void DebugTestCase::Pre()
	{
		std::unique_ptr<ISimulatorBuilder, decltype(&BuilderDeleter)> builder(BuildSimulator(), BuilderDeleter);
		builder->WithNavMesh("Resources/square.nav")
			->WithOp(_opComponent);

		_sim = std::unique_ptr<ISimulatorFacade, decltype(&SimulatorFacadeDeleter)>(builder->Build(), SimulatorFacadeDeleter);


		size_t id = _sim->AddAgent(-1.f, 0.0f, _opComponent, ComponentIds::NAVMESH_ID, -1);
		_sim->SetAgentGoal(id, Point { 1.0f, 0.0f });

		size_t id2 = _sim->AddAgent(0.0f, 0.0f, _opComponent, ComponentIds::NAVMESH_ID, -1);
		//_sim->SetAgentGoal(id2, Point { 0.0f, 5.0f });
	}
}
