#pragma once

namespace FusionCrowd {
struct SimulationStepInfo
{
	size_t rewinds;
	size_t collisions;
	float totalSimulatedTime;
};
}