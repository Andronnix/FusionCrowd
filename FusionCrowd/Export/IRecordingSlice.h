#pragma once

#include "Export/AgentInfo.h"
#include "Export/Config.h"
#include "Export/FCArray.h"

namespace FusionCrowd
{
	extern "C"
	{
		class FUSION_CROWD_API IRecordingSlice
		{
		public:
			virtual float GetTime() const = 0;
			virtual size_t GetAgentCount() const = 0;
			virtual AgentInfo GetAgentInfo(size_t agentId) const = 0;
			virtual void GetAgentIds(FCArray<size_t> & outIds) const = 0;

			virtual ~IRecordingSlice() { }
		};
	}
}
