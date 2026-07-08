#include "AI.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

NavMesh::NavMesh() {
    LOG_DEBUG("NavMesh created");
}

NavMesh::~NavMesh() = default;

void NavMesh::Build(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices) {
    m_Vertices = vertices;
    m_Indices = indices;
    m_IsBuilt = true;
    LOG_DEBUG("NavMesh built with {} vertices and {} indices", vertices.size(), indices.size());
}

bool NavMesh::IsWalkable(const Vector3& position) const {
    return m_IsBuilt;
}

bool NavMesh::Raycast(const Vector3& start, const Vector3& end, Vector3& outHit) const {
    return false;
}

PathFinding::PathFinding() {
    LOG_DEBUG("PathFinding created");
}

PathFinding::~PathFinding() = default;

bool PathFinding::FindPath(const Vector3& start, const Vector3& goal, std::vector<Vector3>& outPath) {
    LOG_DEBUG("Finding path from {} to {}", start.x, goal.x);
    outPath.clear();
    outPath.push_back(start);
    outPath.push_back(goal);
    m_Path = outPath;
    return true;
}

BehaviorTree::BehaviorTree() {
    LOG_DEBUG("BehaviorTree created");
}

BehaviorTree::~BehaviorTree() = default;

BehaviorTree::NodeStatus BehaviorTree::Execute() {
    if (m_Root) {
        return m_Root->Execute();
    }
    return NodeStatus::Failure;
}

AIController::AIController() {
    LOG_DEBUG("AIController created");
}

AIController::~AIController() = default;

void AIController::Update(float deltaTime) {
    if (m_BehaviorTree) {
        m_BehaviorTree->Execute();
    }
}

void AIController::MoveTo(const Vector3& target) {
    m_TargetPosition = target;
    if (m_NavMesh) {
        m_NavMesh->FindPath(Vector3::Zero(), target, m_Path);
    }
}

} // namespace Luminary
