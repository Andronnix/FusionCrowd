#include "PotentialTouristGroup.h"

#include "Math/Shapes/ConeShape.h"
#include "Math/consts.h"

using namespace DirectX::SimpleMath;

namespace FusionCrowd
{
	PotentialTouristGroup::PotentialTouristGroup(size_t id, size_t dummyId, const AgentSpatialInfo & leader) : IGroup(id, dummyId)
	{
		_leaderId = leader.id;
	}

	size_t PotentialTouristGroup::GetSize() const {
		return _agents.size() + 1;
	}

	bool PotentialTouristGroup::Contains(size_t agentId) const
	{
		return _leaderId == agentId || _agents.find(agentId) != _agents.end();
	}

	void PotentialTouristGroup::AddAgent(size_t agentId, AgentSpatialInfo& info)
	{
		if(Contains(agentId))
			return;

		_agents.insert(agentId);
	}

	void PotentialTouristGroup::RemoveAgent(size_t agentId)
	{
		_agents.erase(agentId);
	}

	std::vector<size_t> PotentialTouristGroup::GetAgents() const
	{
		std::vector<size_t> result(_agents.begin(), _agents.end());
		result.push_back(_leaderId);
		return result;
	}

	float PotentialTouristGroup::GetRadius() const
	{
		return _agents.size();
	}

	void PotentialTouristGroup::SetAgentPrefVelocity(const AgentSpatialInfo& dummyInfo, AgentSpatialInfo& agentInfo, float timeStep) const
	{
		if(agentInfo.id == _leaderId)
		{
			agentInfo.prefVelocity.setSingle(dummyInfo.GetPos() - agentInfo.GetPos());
			agentInfo.prefVelocity.setSpeed(agentInfo.prefSpeed);
			return;
		}

		Math::ConeShape c(dummyInfo.GetPos(), 8, Math::PI / 2);

		float relativeRad = Math::orientToRad(dummyInfo.GetOrient()) - Math::orientToRad(agentInfo.GetOrient());
		Vector2 rotPos = Math::rotate(agentInfo.GetPos(), -relativeRad - Math::PI);

		const float minDist = 2;

		if(c.containsPoint(rotPos))
		{
			// Set so agent will go along with group
			agentInfo.prefVelocity.setSingle(dummyInfo.prefVelocity.getPreferredVel());
			if(Vector2::Distance(agentInfo.GetPos(), dummyInfo.GetPos()) >= minDist)
			{
				agentInfo.prefVelocity.setSpeed(dummyInfo.GetVel().Length());
			} else
			{
				agentInfo.prefVelocity.setSpeed(0);
			}
			return;
		}

		// Outside the cone, let's go to the cone
		Vector2 target = dummyInfo.GetPos() - minDist * dummyInfo.GetOrient();

		agentInfo.prefVelocity.setSingle(target - agentInfo.GetPos());
		agentInfo.prefVelocity.setSpeed(agentInfo.prefSpeed);
	}

	Vector2 PotentialTouristGroup::GetRelativePos(size_t agentId) const
	{
		assert(false && "This should never be called.");

		return Vector2();
	}
}
