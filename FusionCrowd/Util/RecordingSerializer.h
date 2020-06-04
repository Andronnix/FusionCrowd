#pragma once

#include "Navigation/OnlineRecording/OnlineRecording.h"

#include <string>

namespace FusionCrowd
{
	namespace Recordings
	{
		void Serialize(const OnlineRecording & rec, const std::string destFilePath);
	}
}
