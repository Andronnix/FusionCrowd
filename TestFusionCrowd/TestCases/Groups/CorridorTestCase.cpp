#include "pch.h"
#include "CorridorTestCase.h"

#include "TestCases/Utils.h"

#include "Export/ComponentId.h"

using namespace FusionCrowd;

namespace TestFusionCrowd
{
	CorridorTestCase::CorridorTestCase(size_t groupsNum, size_t steps)
	: ITestCase(groupsNum * 4, steps, true), _opComponent(ComponentIds::ORCA_ID), _groupsNum(groupsNum)
	{ }

	void CorridorTestCase::SetUpGroup(float gx, float gy)
	{
		size_t groupId = _sim->AddGridGroup(gx, gy, 2, 0.5);
		size_t a1 = _sim->AddAgent(gx, gy - 0.3, ComponentIds::ORCA_ID, ComponentIds::NAVMESH_ID, ComponentIds::NO_COMPONENT);
		size_t a2 = _sim->AddAgent(gx, gy + 0.3, ComponentIds::ORCA_ID, ComponentIds::NAVMESH_ID, ComponentIds::NO_COMPONENT);
		_sim->AddAgentToGroup(a1, groupId);
		_sim->AddAgentToGroup(a2, groupId);
		_sim->SetGroupGoal(groupId, 30 - gx, gy);
	}

	void CorridorTestCase::Pre()
	{
		std::unique_ptr<ISimulatorBuilder, decltype(&BuilderDeleter)> builder(BuildSimulator(), BuilderDeleter);
		builder->WithNavMesh("Resources/corridor_20x3.nav")
			->WithOp(_opComponent);

		_sim = std::unique_ptr<ISimulatorFacade, decltype(&SimulatorFacadeDeleter)>(builder->Build(), SimulatorFacadeDeleter);

		for(size_t g = 0; g < _groupsNum; g++)
		{
			SetUpGroup(RandFloat(1, 4), RandFloat(1, 8)); //Left to right
			SetUpGroup(RandFloat(26, 29), RandFloat(1, 8)); //Right to left
		}
	}
}
