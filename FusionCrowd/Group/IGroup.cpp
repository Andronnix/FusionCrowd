#include "IGroup.h"

using namespace DirectX::SimpleMath;

namespace FusionCrowd
{
	IGroup::IGroup(size_t id, size_t dummyId) : _id(id), _dummyAgentId(dummyId) { }

	void IGroup::SetAgentPrefVelocity(const AgentSpatialInfo & dummyInfo, AgentSpatialInfo & agentInfo, float timeStep) const
	{
		const float rot = atan2f(dummyInfo.GetOrient().y, dummyInfo.GetOrient().x);
		const Vector2 relativePos = GetRelativePos(agentInfo.id);
		const Vector2 rotRelativePos = Math::rotate(relativePos, rot);
		const Vector2 targetPos = dummyInfo.GetPos() + rotRelativePos + dummyInfo.velNew * timeStep;

		Vector2 dir = targetPos - agentInfo.GetPos();

		float speed = dir.Length() / timeStep;
		if(speed > agentInfo.maxSpeed)
			speed = agentInfo.maxSpeed;

		agentInfo.prefVelocity.setSpeed(speed);
		agentInfo.prefVelocity.setSingle(dir);
	}

	size_t IGroup::GetDummyId() const
	{
		return _dummyAgentId;
	}

	size_t IGroup::GetId() const
	{
		return _id;
	}
}
