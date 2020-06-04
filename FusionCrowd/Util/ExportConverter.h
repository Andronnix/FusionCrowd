#pragma once

#include "Agent.h"
#include "Export/AgentInfo.h"
#include "Navigation/AgentSpatialInfo.h"

namespace FusionCrowd
{
	AgentInfo fromSpatialInfo(const AgentSpatialInfo& info, const Agent & agent);
}
