#include "OnlineRecording.h"

#include <map>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>

#include "Util/RecordingSerializer.h"
#include "Navigation/AgentSpatialInfo.h"

namespace FusionCrowd
{
	OnlineRecording::OnlineRecording() : m_currentSlice(std::vector<AgentInfo>(), 0)
	{ }

	size_t OnlineRecording::GetSlicesCount() const {
		return m_slices.size();
	}

	void OnlineRecording::GetTimeSpan(TimeSpan & outTimeSpan) const
	{
		std::copy(m_snapshotTimes.begin(), m_snapshotTimes.end(), outTimeSpan.begin());
	}

	const OnlineRecordingSlice & OnlineRecording::GetSlice(float time) const
	{
		assert(time >= 0 && "Time must be non-negative");

		if(time >= m_currentTime)
			return m_currentSlice;

		auto result = std::lower_bound(m_snapshotTimes.begin(), m_snapshotTimes.end(), time);

		if(result == m_snapshotTimes.end())
			result = std::prev(m_snapshotTimes.end());

		size_t i = result - m_snapshotTimes.begin();

		auto it = m_slices.begin();
		std::advance(it, i);
		return *it;
	}

	const OnlineRecordingSlice & OnlineRecording::GetCurrentSlice() const {
		return m_currentSlice;
	}

	size_t OnlineRecording::GetAgentCount() const
	{
		return std::max(m_prevAgentCount, m_currentSlice.GetAgentCount());
	}

	void OnlineRecording::MakeRecord(std::vector<AgentInfo> && agentsInfos, float timeStep)
	{
		assert(timeStep >= 0 && "Time step must be positive");

		m_snapshotTimes.push_back(m_currentTime);
		m_slices.push_back(m_currentSlice);
		m_prevAgentCount = std::max(m_prevAgentCount, m_currentSlice.GetAgentCount());

		m_currentTime += timeStep;
		m_currentSlice = OnlineRecordingSlice(std::move(agentsInfos), m_currentTime);
	}
};
