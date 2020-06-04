#pragma once

#include "Config.h"
#include "ComponentId.h"

namespace FusionCrowd
{
	extern "C"
	{
		struct FUSION_CROWD_API AgentInfo
		{
			enum FUSION_CROWD_API Type {
				Agent, Group
			};

			size_t id;

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
