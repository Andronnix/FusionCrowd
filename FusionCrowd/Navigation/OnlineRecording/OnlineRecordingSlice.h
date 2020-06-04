#pragma once

#include <map>

#include "Export/IRecordingSlice.h"

namespace FusionCrowd
{
	class OnlineRecordingSlice : public IRecordingSlice
	{
	public:
		OnlineRecordingSlice(std::vector<AgentInfo> agentsInfos, float newTime);

		OnlineRecordingSlice(const OnlineRecordingSlice & other);
		OnlineRecordingSlice(OnlineRecordingSlice && other);

		OnlineRecordingSlice& operator=(const OnlineRecordingSlice & other);
		OnlineRecordingSlice& operator=(OnlineRecordingSlice && other);

		float GetTime() const override;
		size_t GetAgentCount() const override;
		AgentInfo GetAgentInfo(size_t agentId) const override;
		void GetAgentIds(FCArray<size_t> & outIds) const override;

	private:
		float _time;
		std::map<size_t, AgentInfo> m_agentInfos;
	};
}
