#include "ExportConverter.h"

namespace FusionCrowd
{
	AgentInfo fromSpatialInfo(const AgentSpatialInfo& info, const Agent & agent)
	{
		auto & g = agent.currentGoal;

		ComponentId op = -1, tactic = -1, strat = -1;
		if(!agent.opComponent.expired())
		{
			op = agent.opComponent.lock()->GetId();
		}

		if(!agent.tacticComponent.expired())
		{
			tactic = agent.tacticComponent.lock()->GetId();
		}

		if(!agent.stratComponent.expired())
		{
			strat = agent.stratComponent.lock()->GetId();
		}

		return AgentInfo {
			agent.id,
			info.GetPos().x, info.GetPos().y,
			info.GetVel().x, info.GetVel().y,
			info.GetOrient().x, info.GetOrient().y,
			info.radius,
			op, tactic, strat,
			g.getCentroid().x, g.getCentroid().y
		};
	}
}
