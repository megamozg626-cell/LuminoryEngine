#pragma once

#include "../Math/Vector3.hpp"
#include <vector>
#include <string>
#include <memory>

namespace Luminary {

class NavMesh {
public:
    NavMesh();
    ~NavMesh();

    void Build(const std::vector<Vector3>& vertices, const std::vector<uint32_t>& indices);
    bool IsWalkable(const Vector3& position) const;
    bool Raycast(const Vector3& start, const Vector3& end, Vector3& outHit) const;

private:
    std::vector<Vector3> m_Vertices;
    std::vector<uint32_t> m_Indices;
    bool m_IsBuilt = false;
};

class PathFinding {
public:
    PathFinding();
    ~PathFinding();

    bool FindPath(const Vector3& start, const Vector3& goal, std::vector<Vector3>& outPath);
    std::vector<Vector3> GetPath() const { return m_Path; }

private:
    std::vector<Vector3> m_Path;
};

class BehaviorTree {
public:
    enum class NodeStatus {
        Running,
        Success,
        Failure
    };

    class BehaviorNode {
    public:
        virtual ~BehaviorNode() = default;
        virtual NodeStatus Execute() = 0;
    };

    BehaviorTree();
    ~BehaviorTree();

    void SetRootNode(std::shared_ptr<BehaviorNode> root) { m_Root = root; }
    NodeStatus Execute();

private:
    std::shared_ptr<BehaviorNode> m_Root;
};

class AIController {
public:
    AIController();
    ~AIController();

    void SetNavMesh(std::shared_ptr<NavMesh> navMesh) { m_NavMesh = navMesh; }
    void SetBehaviorTree(std::shared_ptr<BehaviorTree> tree) { m_BehaviorTree = tree; }

    void Update(float deltaTime);
    void MoveTo(const Vector3& target);

private:
    std::shared_ptr<NavMesh> m_NavMesh;
    std::shared_ptr<BehaviorTree> m_BehaviorTree;
    Vector3 m_TargetPosition = Vector3::Zero();
    std::vector<Vector3> m_Path;
};

} // namespace Luminary
