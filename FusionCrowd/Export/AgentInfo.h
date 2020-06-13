#pragma once

#include "Config.h"
#include "ComponentId.h"

namespace FusionCrowd
{
	extern "C"
	{
		enum class FUSION_CROWD_API AgentType {
			Agent, Group
		};

		struct FUSION_CROWD_API AgentInfo
		{
			size_t id;
			AgentType type;

			float posX, posY;
			float velX, velY;
			float orientX, orientY;
			float radius;

			ComponentId opCompId;
			ComponentId tacticCompId;
			ComponentId stratCompId;

			float goalX, goalY;
		};
	}
}
