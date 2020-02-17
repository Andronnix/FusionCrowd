#pragma once

#include <vector>
#include <memory>

#include "Agent.h"

#include "Export/ComponentId.h"
#include "Export/Export.h"
#include "Export/FCArray.h"
#include "Export/IStrategyComponent.h"

#include "StrategyComponent/Goal/Goal.h"
#include "TacticComponent/ITacticComponent.h"
#include "OperationComponent/IOperationComponent.h"
#include "Navigation/NavSystem.h"

#include "Util/spimpl.h"
#include "Math/Util.h"

namespace FusionCrowd
{
	class NavMeshComponent;

	class Simulator
	{
	public:
		Simulator();

		Simulator & AddOpModel(std::shared_ptr<IOperationComponent> operComponent);
		Simulator & AddTactic(std::shared_ptr<ITacticComponent> tacticComponent);
		Simulator & AddStrategy(std::shared_ptr<IStrategyComponent> strategyComponent);
		Simulator & UseNavSystem(std::shared_ptr<NavSystem> system);

		void SetNavSystem(std::shared_ptr<NavSystem> navSystem);

		bool DoStep(float timeStep);

		bool SetOperationComponent(size_t agentId, ComponentId newOperationComponent);
		bool SetTacticComponent(size_t agentId, ComponentId newTactic);
		bool SetStrategyComponent(size_t agentId, ComponentId newStrategyComponent);

		void SetAgentStrategyParam(size_t agentId, ComponentId strategyId, ModelAgentParams & params);
		IStrategyComponent* GetStrategy(ComponentId strategyId) const;

		AgentSpatialInfo & GetSpatialInfo(size_t agentId);

		IRecording & GetRecording();
		void SetIsRecording(bool isRecording);

		float GetElapsedTime();

		size_t GetAgentCount() const;
		const Goal & GetAgentGoal(size_t agentId) const;

		size_t AddAgent(DirectX::SimpleMath::Vector2 pos);
		size_t AddAgent(
			float x, float y,
			ComponentId opId,
			ComponentId strategyId,
			ComponentId tacticId
		);

		void SetAgentGoal(size_t agentId, DirectX::SimpleMath::Vector2 goalPos);

		Agent & GetAgent(size_t id);

		FCArray<AgentInfo> GetAgentsInfo();

		// Must: output.len >= GetAgentCount();
		bool GetAgentsInfo(FCArray<AgentInfo> & output);

		NavSystem* GetNavSystem() const;
	private:
		class SimulatorImpl;

		spimpl::unique_impl_ptr<SimulatorImpl> pimpl;
	};
}
