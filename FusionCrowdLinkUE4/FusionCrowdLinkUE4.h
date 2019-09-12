#define LINKFUSIONCROWD_API __declspec(dllexport)

#include <memory>

#include "UE4StrategyProxy.h"

namespace FusionCrowd
{
	class Simulator;
	class NavMeshComponent;
	namespace Karamouzas
	{
		class KaramouzasComponent;
	}
	namespace ORCA
	{
		class ORCAComponent;
	}
	namespace PedVO
	{
		class PedVOComponent;
	}
	namespace Helbing
	{
		class HelbingComponent;
	}
}

struct agentInfo
{
	size_t id;
	float* pos;
	float* orient;
	float* vel;
	float radius;
	char* opCompName;
};

class FusionCrowdLinkUE4
{
#pragma warning(disable : 4996)
public:
	LINKFUSIONCROWD_API FusionCrowdLinkUE4();
	LINKFUSIONCROWD_API ~FusionCrowdLinkUE4();

	LINKFUSIONCROWD_API void SetGoal(size_t agentId, const float * goalPos);
	LINKFUSIONCROWD_API void SetOperationModel(size_t agentId, const char * name);
	LINKFUSIONCROWD_API void StartFusionCrowd(char* navMeshDir);
	LINKFUSIONCROWD_API int GetAgentCount();
	LINKFUSIONCROWD_API size_t AddAgent(const float * agentPos, const float * goalPos, const char * opComponent);
	LINKFUSIONCROWD_API void AddAgents(int agentsCount);
	LINKFUSIONCROWD_API void GetPositionAgents(agentInfo* agentsPos);
	LINKFUSIONCROWD_API void UpdateNav(float x, float y);
	LINKFUSIONCROWD_API void GetTrinagles();
	LINKFUSIONCROWD_API void GetNewNode(float* outer, float* contour, int outCount, int conturCount, int*& triangels, int& triangelsCount, float*& vertexs, int& vertexsCount);
	LINKFUSIONCROWD_API void AddNode(int* idNodes, int idNodesCount, float* contour, int conturCount);
	LINKFUSIONCROWD_API void GetIntersectionNode(float* contour, int conturCount, int*& idNodes, int& idNodesCount);
	//LINKFUSIONCROWD_API int CheckObstacle(float** countur, int counterVertex);

private:
	FusionCrowd::Simulator* sim;
	std::shared_ptr<UE4StrategyProxy> _strategy;
	std::shared_ptr<FusionCrowd::Karamouzas::KaramouzasComponent> kComponent;
	std::shared_ptr<FusionCrowd::ORCA::ORCAComponent> orcaComponent;
	std::shared_ptr<FusionCrowd::PedVO::PedVOComponent> pedvoComponent;
	std::shared_ptr<FusionCrowd::Helbing::HelbingComponent> helbingComponent;
	std::shared_ptr<FusionCrowd::NavMeshComponent> navMeshTactic;
	int agentsCount;
	char* navMeshPath;
};