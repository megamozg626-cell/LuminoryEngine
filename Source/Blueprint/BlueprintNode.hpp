#pragma once

#include <memory>
#include <vector>
#include <string>
#include <map>
#include <functional>

namespace Luminary {

class BlueprintNode {
public:
    enum class NodeType {
        Input,
        Output,
        Math,
        Logic,
        Branch,
        Loop,
        Function,
        Event,
        Variable,
        Custom
    };

    enum class PinType {
        Execution,
        Boolean,
        Integer,
        Float,
        Vector3,
        String,
        Object
    };

    struct Pin {
        std::string Name;
        PinType Type;
        bool bIsInput = true;
        std::shared_ptr<class BlueprintNode> LinkedNode;
        int32_t LinkedPinIndex = -1;
    };

    BlueprintNode(NodeType type, const std::string& name);
    virtual ~BlueprintNode();

    NodeType GetType() const { return m_Type; }
    const std::string& GetName() const { return m_Name; }
    uint32_t GetNodeID() const { return m_NodeID; }

    void AddInputPin(const std::string& name, PinType type);
    void AddOutputPin(const std::string& name, PinType type);

    const std::vector<Pin>& GetInputPins() const { return m_InputPins; }
    const std::vector<Pin>& GetOutputPins() const { return m_OutputPins; }

    void ConnectPin(std::shared_ptr<BlueprintNode> targetNode, uint32_t outputPinIndex, uint32_t targetInputPinIndex);
    void DisconnectPin(uint32_t pinIndex);

    virtual void Execute() {}
    virtual void* GetPinValue(uint32_t pinIndex) { return nullptr; }

private:
    static uint32_t s_NodeIDCounter;
    uint32_t m_NodeID;
    NodeType m_Type;
    std::string m_Name;
    std::vector<Pin> m_InputPins;
    std::vector<Pin> m_OutputPins;
};

class BlueprintGraph {
public:
    BlueprintGraph(const std::string& name = "Blueprint");
    ~BlueprintGraph();

    const std::string& GetName() const { return m_Name; }

    std::shared_ptr<BlueprintNode> AddNode(BlueprintNode::NodeType type, const std::string& name);
    void RemoveNode(uint32_t nodeID);
    std::shared_ptr<BlueprintNode> FindNode(uint32_t nodeID) const;

    const std::vector<std::shared_ptr<BlueprintNode>>& GetNodes() const { return m_Nodes; }

    void ExecuteGraph();
    void Compile();

private:
    std::string m_Name;
    std::vector<std::shared_ptr<BlueprintNode>> m_Nodes;
    std::map<uint32_t, std::shared_ptr<BlueprintNode>> m_NodeMap;
    bool m_IsCompiled = false;
};

} // namespace Luminary
