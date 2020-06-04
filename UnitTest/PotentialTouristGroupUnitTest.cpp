#include "stdafx.h"
#include "CppUnitTest.h"


#include "Group/PotentialTouristGroup.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

using namespace FusionCrowd;

using namespace DirectX::SimpleMath;

namespace UnitTest
{
	TEST_CLASS(PotentialTouristGroupUnitTest)
	{
	public:
		TEST_METHOD(PotentialTouristGroup__DoesNotContainOtherAgent)
		{
			const size_t dummyId = 1;

			AgentSpatialInfo leader;
			leader.id = 10;

			auto group = PotentialTouristGroup(0, dummyId, leader);

			Assert::IsFalse(group.Contains(dummyId + 1), L"Contains other agent");
		}

		TEST_METHOD(PotentialTouristGroup__ContainsLeader)
		{
			AgentSpatialInfo leader;
			leader.id = 10;

			auto group = PotentialTouristGroup(0, 1, leader);


			Assert::IsTrue(group.Contains(leader.id), L"Must contain leader");
		}

		TEST_METHOD(PotentialTouristGroup__DoesNotContainDummy)
		{
			const size_t dummyId = 1;

			AgentSpatialInfo leader;
			leader.id = 10;

			auto group = PotentialTouristGroup(0, dummyId, leader);


			Assert::IsFalse(group.Contains(dummyId), L"Mustn't contain dummy");
		}

		TEST_METHOD(PotentialTouristGroup__CanAddAgents)
		{
			const size_t dummyId = 1;

			AgentSpatialInfo leader;
			leader.id = 10;

			auto group = PotentialTouristGroup(0, dummyId, leader);

			for(int id = 20; id < 30; id++) {
				AgentSpatialInfo agt;
				agt.id = id;
				group.AddAgent(agt.id, agt);
			}

			for(int id = 20; id < 30; id++) {
				Assert::IsTrue(group.Contains(id), L"Must contain all agents");
			}
			Assert::IsTrue(group.GetSize() == 11, L"Must count all agents");
		}

		TEST_METHOD(PotentialTouristGroup__CanRemoveAgents)
		{
			const size_t dummyId = 1;

			AgentSpatialInfo leader;
			leader.id = 10;

			auto group = PotentialTouristGroup(0, dummyId, leader);

			for(int id = 20; id < 30; id++) {
				AgentSpatialInfo agt;
				agt.id = id;
				group.AddAgent(agt.id, agt);
			}


			for(int id = 20; id < 30; id++) {
				Assert::IsTrue(group.Contains(id), L"Must contain all agents");
			}

			for(int id = 20; id < 30; id++) {
				group.RemoveAgent(id);
			}

			for(int id = 20; id < 30; id++) {
				Assert::IsFalse(group.Contains(id), L"Agent must be absent");
			}

			Assert::IsTrue(group.GetSize() == 1, L"Only leader must be left");
		}

		TEST_METHOD(PotentialTouristGroup__SetsPrefSpeedOnSameAxis)
		{
			AgentSpatialInfo leader;
			leader.id = 10;

			AgentSpatialInfo dummy;
			dummy.id = 1;
			dummy.SetPos(DirectX::SimpleMath::Vector2());

			auto group = PotentialTouristGroup(0, dummy.id, leader);

			AgentSpatialInfo agent;
			agent.id = 20;
			agent.SetPos(DirectX::SimpleMath::Vector2(-3, 0));
			agent.prefVelocity.setSpeed(10);

			group.AddAgent(agent.id, agent);

			group.SetAgentPrefVelocity(dummy, agent, 1);

			Assert::AreEqual(agent.prefVelocity.getSpeed(), 0.f, L"Speed must be zero");

			agent.SetPos(DirectX::SimpleMath::Vector2(-10, 0));
			agent.prefVelocity.setSpeed(0);

			group.SetAgentPrefVelocity(dummy, agent, 1);

			Assert::AreEqual(agent.prefVelocity.getSpeed(), agent.prefSpeed, L"Must want to move");

			agent.SetPos(DirectX::SimpleMath::Vector2(0, 0));
			agent.prefVelocity.setSpeed(0);

			group.SetAgentPrefVelocity(dummy, agent, 1);

			Assert::AreEqual(agent.prefVelocity.getSpeed(), agent.prefSpeed, L"Must want to move");
		}

		TEST_METHOD(PotentialTouristGroup__SetsPrefDirectionOnSameAxis)
		{
			AgentSpatialInfo leader;
			leader.id = 10;

			AgentSpatialInfo dummy;
			dummy.id = 1;
			dummy.SetPos(DirectX::SimpleMath::Vector2());

			auto group = PotentialTouristGroup(0, dummy.id, leader);

			AgentSpatialInfo agent;
			agent.id = 20;

			agent.SetPos(DirectX::SimpleMath::Vector2(-10, 0));
			agent.prefVelocity.setSpeed(0);

			group.SetAgentPrefVelocity(dummy, agent, 1);

			Assert::AreEqual(agent.prefVelocity.getPreferred().x, 1.f, L"Must want to move along X axis");
			Assert::AreEqual(agent.prefVelocity.getPreferred().y, 0.f, L"Must want to move along X axis");

			agent.SetPos(DirectX::SimpleMath::Vector2(0, 0));
			agent.prefVelocity.setSpeed(0);

			group.SetAgentPrefVelocity(dummy, agent, 1);

			Assert::AreEqual(agent.prefVelocity.getPreferred().x, -1.f, L"Must want to move along X axis in opposite direction");
			Assert::AreEqual(agent.prefVelocity.getPreferred().y, 0.f, L"Must want to move along X axis in opposite direction");
		}

		TEST_METHOD(PotentialTouristGroup__SetsLeaderPrefDirection)
		{
			AgentSpatialInfo leader;
			leader.id = 10;

			AgentSpatialInfo dummy;
			dummy.id = 1;
			dummy.SetPos(DirectX::SimpleMath::Vector2());

			auto group = PotentialTouristGroup(0, dummy.id, leader);

			leader.SetPos(Vector2(-1, 0));
			group.SetAgentPrefVelocity(dummy, leader, 1);

			Assert::AreEqual(leader.prefVelocity.getPreferred().x, 1.f, L"Must want to move to group center");
			Assert::AreEqual(leader.prefVelocity.getPreferred().y, 0.f, L"Must want to move to group center");

			leader.SetPos(Vector2(1, 0));
			group.SetAgentPrefVelocity(dummy, leader, 1);

			Assert::AreEqual(leader.prefVelocity.getPreferred().x, -1.f, L"Must want to move to group center");
			Assert::AreEqual(leader.prefVelocity.getPreferred().y, 0.f, L"Must want to move to group center");

			leader.SetPos(Vector2(0, -1));
			group.SetAgentPrefVelocity(dummy, leader, 1);

			Assert::AreEqual(leader.prefVelocity.getPreferred().x, 0.f, L"Must want to move to group center");
			Assert::AreEqual(leader.prefVelocity.getPreferred().y, 1.f, L"Must want to move to group center");

			leader.SetPos(Vector2(0, 1));
			group.SetAgentPrefVelocity(dummy, leader, 1);

			Assert::AreEqual(leader.prefVelocity.getPreferred().x, 0.f, L"Must want to move to group center");
			Assert::AreEqual(leader.prefVelocity.getPreferred().y, -1.f, L"Must want to move to group center");
		}
	};
}
