#pragma once

#include <memory>

#include "Export/AgentInfo.h"
#include "Export/IRecording.h"
#include "Navigation/OnlineRecording/OnlineRecordingSlice.h"

#include "Export/FCArray.h"

namespace FusionCrowd
{
	class OnlineRecording : public IRecording
	{
	public:
		OnlineRecording();

		size_t GetSlicesCount() const override;
		void GetTimeSpan(TimeSpan & outTimeSpan) const override;
		const OnlineRecordingSlice & GetSlice(float time) const override;
		const OnlineRecordingSlice & GetCurrentSlice() const override;

		size_t GetAgentCount() const;

		void MakeRecord(std::vector<AgentInfo> && agentsInfos, float timeStep);
	private:
		float m_currentTime = 0;
		OnlineRecordingSlice m_currentSlice;

		size_t m_prevAgentCount = 0;
		std::vector<float> m_snapshotTimes;

		std::vector<OnlineRecordingSlice> m_slices;
	};
}
