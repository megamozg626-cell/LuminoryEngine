#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>

namespace Luminary {

class MaterialNode : public std::enable_shared_from_this<MaterialNode> {
public:
    enum class NodeType {
        Texture,
        TextureSample,
        Multiply,
        Add,
        Subtract,
        Divide,
        Power,
        Lerp,
        Normal,
        Noise,
        Mask,
        Time,
        Sin,
        Cos,
        Panner,
        UV,
        WorldPosition,
        CameraPosition,
        VertexColor,
        Scalar,
        Vector,
        If,
        Clamp,
        OneMinus,
        Append,
        Switch,
        MaterialOutput,
        Custom
    };

    struct Connection {
        std::shared_ptr<MaterialNode> TargetNode;
        uint32_t OutputIndex = 0;
        uint32_t InputIndex = 0;
    };

    MaterialNode(NodeType type, const std::string& name);
    virtual ~MaterialNode();

    NodeType GetType() const { return m_Type; }
    const std::string& GetName() const { return m_Name; }
    uint32_t GetNodeID() const { return m_NodeID; }

    void AddInputConnection(std::shared_ptr<MaterialNode> sourceNode, uint32_t sourceOutput, uint32_t targetInput);
    void AddOutputConnection(std::shared_ptr<MaterialNode> targetNode, uint32_t sourceOutput, uint32_t targetInput);

    const std::vector<Connection>& GetInputConnections() const { return m_InputConnections; }
    const std::vector<Connection>& GetOutputConnections() const { return m_OutputConnections; }

    virtual void Evaluate() {}

private:
    static uint32_t s_NodeIDCounter;
    uint32_t m_NodeID;
    NodeType m_Type;
    std::string m_Name;
    std::vector<Connection> m_InputConnections;
    std::vector<Connection> m_OutputConnections;
};

class MaterialGraph {
public:
    MaterialGraph(const std::string& name = "Material");
    ~MaterialGraph();

    const std::string& GetName() const { return m_Name; }

    std::shared_ptr<MaterialNode> AddNode(MaterialNode::NodeType type, const std::string& name);
    void RemoveNode(uint32_t nodeID);
    std::shared_ptr<MaterialNode> FindNode(uint32_t nodeID) const;

    const std::vector<std::shared_ptr<MaterialNode>>& GetNodes() const { return m_Nodes; }

    void Evaluate();
    void Compile();
    bool Save(const std::string& path);
    bool Load(const std::string& path);

private:
    std::string m_Name;
    std::vector<std::shared_ptr<MaterialNode>> m_Nodes;
    std::map<uint32_t, std::shared_ptr<MaterialNode>> m_NodeMap;
    bool m_IsCompiled = false;
};

} // namespace Luminary
