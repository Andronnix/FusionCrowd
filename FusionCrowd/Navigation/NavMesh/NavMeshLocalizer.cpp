#include "NavMeshLocalizer.h"

#include "NavMeshNode.h"
#include "TacticComponent/Path/PathPlanner.h"
#include "TacticComponent/Path/PortalPath.h"

#include <limits>
#include <iostream>

using namespace DirectX::SimpleMath;

namespace FusionCrowd
{
	void NavMeshLocation::setNode(unsigned int nodeID)
	{
		if (_hasPath)
		{
			_path = nullptr;

		}
		_hasPath = false;
		_nodeID = nodeID;
	}

	void NavMeshLocation::clearPath()
	{
		if (_hasPath)
		{
			unsigned int node = _path->getNode();
			_path = nullptr;
			_hasPath = false;
			_nodeID = (size_t)node;
		}
	}

	unsigned int NavMeshLocation::getNode() const
	{
		//TODO: NavMesh nodes should simply be size_ts and NOT unsigned ints.
		if (_hasPath)
		{
			return (unsigned int)_path->getNode();
		}
		else
		{
			return (unsigned int)_nodeID;
		}
	}

	void NavMeshLocation::setPath(std::shared_ptr<PortalPath> path)
	{
		_path = path;
		_hasPath = true;
		_nodeID = std::numeric_limits<size_t>::max();
	}

	std::shared_ptr<PortalPath> NavMeshLocation::getPath()
	{
		if(_hasPath)
			return _path;
		else
			return nullptr;
	}

	//
	//

	void NavMeshLocalizer::updateAgentPosition(size_t agentId, const unsigned int oldLoc, unsigned int newLoc)
	{
		if (newLoc != oldLoc)
		{
			if (newLoc == NavMeshLocation::NO_NODE)
			{
				newLoc = static_cast<unsigned int>(_navMesh->getNodeCount());
			}

			// remove the agent from the set for oldLoc and place it in newLoc
#pragma omp critical( NAV_MESH_LOCALIZER_MOVE_AGENT )
			{
				if (oldLoc != NavMeshLocation::NO_NODE)
				{
					OccupantSetItr fromItr = _nodeOccupants[oldLoc].find(agentId);
					if (fromItr != _nodeOccupants[oldLoc].end())
					{
						_nodeOccupants[oldLoc].erase(fromItr);
					}
					else if (oldLoc != NavMeshLocation::NO_NODE)
					{
						const size_t NCOUNT = _navMesh->getNodeCount();
						for (size_t i = 0; i < NCOUNT; ++i)
						{
							fromItr = _nodeOccupants[i].find(agentId);
							if (fromItr != _nodeOccupants[i].end())
							{
								_nodeOccupants[i].erase(fromItr);
								break;
							}
						}
					}
				}
				_nodeOccupants[newLoc].insert(agentId);
			}
		}
	}

	NavMeshLocalizer::NavMeshLocalizer(const std::string& name, bool usePlanner) : _navMesh(0x0), _trackAll(false), _planner(0x0)
	{
		std::ifstream f;
		f.open(name, std::ios::in);

		if (f.is_open())
		{
			_navMesh = NavMesh::Load(f);
		} else
		{
			throw std::ios_base::failure("Can't load navmesh");
		}

		const size_t NODE_COUNT = _navMesh->getNodeCount();
		_nodeOccupants = new OccupantSet[NODE_COUNT + 1];

		if (usePlanner)
		{
			std::shared_ptr<PathPlanner> planner = std::make_shared<PathPlanner>(_navMesh);
			setPlanner(planner);
		}

		std::vector<QuadTree::Box> nodeBoxes;
		for(size_t nodeId = 0; nodeId < NODE_COUNT; nodeId++)
		{
			nodeBoxes.push_back({_navMesh->GetNode(nodeId).GetBB(), nodeId });
		}

		_nodeBBTree = std::make_unique<QuadTree>(nodeBoxes);
	}

	NavMeshLocalizer::~NavMeshLocalizer()
	{
		delete[] _nodeOccupants;
	}

	unsigned int NavMeshLocalizer::getNodeId(const Vector2& p) const
	{
		return findNodeBlind(p, 0.f);
	}

	std::vector<size_t> NavMeshLocalizer::findNodesCrossingBB(BoundingBox bb) {
		return _nodeBBTree->GetIntersectingBBIds(bb);
	}

	unsigned int NavMeshLocalizer::findNodeBlind(const Vector2& p, float tgtElev) const
	{
		float elevDiff = 1e6f;
		unsigned int maxNode = NavMeshLocation::NO_NODE;
		for(size_t nodeId : _nodeBBTree->GetContainingBBIds(p))
		{
			const NavMeshNode& node = _navMesh->GetNode(nodeId);
			if (node.containsPoint(p))
			{
				float hDiff = fabs(node.getElevation(p) - tgtElev);
				if (hDiff < elevDiff)
				{
					maxNode = nodeId;
					elevDiff = hDiff;
				}
			}
		}
		return maxNode;
	}

	unsigned int NavMeshLocalizer::findNodeInGroup(const Vector2& p, const std::string& grpName, bool searchAll) const
	{
		unsigned int node = NavMeshLocation::NO_NODE;
		const NMNodeGroup* grp = _navMesh->getNodeGroup(grpName);
		if (grp != 0x0)
		{
			node = findNodeInRange(p, grp->getFirst(), grp->getLast() + 1);

			if (node == NavMeshLocation::NO_NODE && searchAll)
			{
				node = findNodeInRange(p, 0, grp->getFirst());
				if (node == NavMeshLocation::NO_NODE)
				{
					// TODO(curds01) 10/1/2016 - This cast is bad because I can lose precision
					// (after I get 4 billion nodes...)
					const unsigned int TOTAL_NODES = static_cast<unsigned int>(_navMesh->getNodeCount());
					node = findNodeInRange(p, grp->getFirst() + 1, TOTAL_NODES);
				}
			}
		}
		return node;
	}

	unsigned int NavMeshLocalizer::findNodeInRange(const Vector2& p, unsigned int start, unsigned int stop) const
	{
		for (unsigned int n = start; n < stop; ++n)
		{
			const NavMeshNode& node = _navMesh->GetNode(n);
			if (node.containsPoint(p))
			{
				return n;
			}
		}
		return NavMeshLocation::NO_NODE;
	}

	unsigned int NavMeshLocalizer::testNeighbors(const NavMeshNode& node, const Vector2& p) const
	{
		const unsigned int nCount = static_cast<unsigned int>(node.getNeighborCount());
		for (unsigned int n = 0; n < nCount; ++n)
		{
			const NavMeshNode* nbr = node.getNeighbor(n);
			if (nbr->containsPoint(p))
			{
				return nbr->getID();
			}
		}
		return NavMeshLocation::NO_NODE;
	}
}
