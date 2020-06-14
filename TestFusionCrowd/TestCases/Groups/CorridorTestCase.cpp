#include "pch.h"
#include "CorridorTestCase.h"

#include "TestCases/Utils.h"

#include "Export/ComponentId.h"

using namespace FusionCrowd;

namespace TestFusionCrowd
{
	CorridorTestCase::CorridorTestCase(size_t halfNum, float groupFraction, size_t steps)
	: ITestCase(halfNum * 2, steps, true),
		_opComponent(ComponentIds::ORCA_ID),
		_groups(halfNum * groupFraction / 2),
		_singles(halfNum - _groups * 2),
		_busy(std::vector<std::vector<bool>>(10))
	{
		for(size_t y = 0; y < _busy.size(); y++)
			_busy[y] = std::vector<bool>(31);
	}

	void CorridorTestCase::SetUpGroup(float gx, float gy)
	{
		size_t groupId = _sim->AddGridGroup(gx, gy, 2, 0.1);
		size_t a1 = _sim->AddAgent(gx, gy + 0.2, ComponentIds::ORCA_ID, ComponentIds::NAVMESH_ID, ComponentIds::NO_COMPONENT);
		size_t a2 = _sim->AddAgent(gx, gy - 0.2, ComponentIds::ORCA_ID, ComponentIds::NAVMESH_ID, ComponentIds::NO_COMPONENT);
		_sim->AddAgentToGroup(a1, groupId);
		_sim->AddAgentToGroup(a2, groupId);
		_sim->SetGroupGoal(groupId, 30 - gx, gy);

		_busy[(int) gy][(int) gx] = true;
	}

	void CorridorTestCase::SetUpSingle(float x1, float x2, float y1, float y2)
	{
		float x = 0; float y = 0;
		do
		{
			x = RandFloat(x1, x2);
			y = RandFloat(y1, y2);
		}
		while(_busy[(int) y][(int) x]);

		_busy[(int) y][(int) x] = true;

		size_t a = _sim->AddAgent(x, y, ComponentIds::ORCA_ID, ComponentIds::NAVMESH_ID, ComponentIds::NO_COMPONENT);
		_sim->SetAgentGoal(a, Point(30-x, y));
	}

	void CorridorTestCase::Pre()
	{
		std::unique_ptr<ISimulatorBuilder, decltype(&BuilderDeleter)> builder(BuildSimulator(), BuilderDeleter);
		builder->WithNavMesh("Resources/corridor_20x3.nav")
			->WithOp(_opComponent);

		_sim = std::unique_ptr<ISimulatorFacade, decltype(&SimulatorFacadeDeleter)>(builder->Build(), SimulatorFacadeDeleter);

		for(size_t g = 0; g < _groups; g++)
		{
			SetUpGroup(RandFloat(1, 4), RandFloat(1, 8)); //Left to right
			SetUpGroup(RandFloat(26, 29), RandFloat(1, 8)); //Right to left
		}

		for(size_t s = 0; s < _singles; s++)
		{
			SetUpSingle(1, 4, 1, 8); //Left to right
			SetUpSingle(26, 29, 1, 8); //Right to left
		}
	}
}
