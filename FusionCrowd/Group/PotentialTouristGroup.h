#pragma once

#include "IGroup.h"

#include <vector>
#include <unordered_set>

namespace FusionCrowd
{
	class PotentialTouristGroup : public IGroup
	{
	public:
		PotentialTouristGroup(size_t id, size_t dummyId, const AgentSpatialInfo & leader);

	public:
		size_t GetSize() const override;
		bool Contains(size_t agentId) const override;
		void AddAgent(size_t agentId, AgentSpatialInfo& info) override;
		void RemoveAgent(size_t agentId) override;
		std::vector<size_t> GetAgents() const override;
		float GetRadius() const override;

		void SetAgentPrefVelocity(const AgentSpatialInfo& dummyInfo, AgentSpatialInfo& agentInfo, float timeStep) const override;

	protected:
		DirectX::SimpleMath::Vector2 GetRelativePos(size_t agentId) const override;

	private:
		size_t _leaderId;
		std::unordered_set<size_t> _agents;
	};
}
