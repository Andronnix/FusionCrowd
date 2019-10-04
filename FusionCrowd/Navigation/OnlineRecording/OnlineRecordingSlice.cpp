#include "OnlineRecordingSlice.h"

namespace FusionCrowd
{
	OnlineRecordingSlice::OnlineRecordingSlice()
	{
	}

	size_t OnlineRecordingSlice::GetAgentCount() const
	{
		return m_agentInfos.size();
	};

	AgentInfo OnlineRecordingSlice::GetAgentInfo(size_t agentId) const
	{
		return m_agentInfos.at(agentId).ToAgentInfo();
	};

	AgentSpatialInfo& OnlineRecordingSlice::GetInfo(size_t agentId)
	{
		return m_agentInfos[agentId];
	};

	void OnlineRecordingSlice::AddAgent(AgentSpatialInfo info)
	{
		m_agentInfos.insert({info.id, info});
	}

	bool OnlineRecordingSlice::RemoveAgent(size_t agentId)
	{
		return m_agentInfos.erase(agentId) > 0;
	}

	FCArray<size_t> OnlineRecordingSlice::GetAgentIds() const
	{
		FCArray<size_t> ids(m_agentInfos.size());

		size_t i = 0;
		for(auto & p : m_agentInfos)
		{
			ids[i++] = p.first;
		}

		return ids;
	}

	OnlineRecordingSlice::OnlineRecordingSlice(const OnlineRecordingSlice & other) = default;
	OnlineRecordingSlice::OnlineRecordingSlice(OnlineRecordingSlice && other) = default;
	OnlineRecordingSlice& OnlineRecordingSlice::operator=(const OnlineRecordingSlice & other) = default;
	OnlineRecordingSlice& OnlineRecordingSlice::operator=(OnlineRecordingSlice && other) = default;
}
