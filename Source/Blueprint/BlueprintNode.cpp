#include "BlueprintNode.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

uint32_t BlueprintNode::s_NodeIDCounter = 0;

BlueprintNode::BlueprintNode(NodeType type, const std::string& name)
    : m_NodeID(s_NodeIDCounter++), m_Type(type), m_Name(name) {
    LOG_DEBUG("BlueprintNode created: {} (ID: {})", name, m_NodeID);
}

BlueprintNode::~BlueprintNode() = default;

void BlueprintNode::AddInputPin(const std::string& name, PinType type) {
    Pin pin;
    pin.Name = name;
    pin.Type = type;
    pin.bIsInput = true;
    m_InputPins.push_back(pin);
}

void BlueprintNode::AddOutputPin(const std::string& name, PinType type) {
    Pin pin;
    pin.Name = name;
    pin.Type = type;
    pin.bIsInput = false;
    m_OutputPins.push_back(pin);
}

void BlueprintNode::ConnectPin(std::shared_ptr<BlueprintNode> targetNode, uint32_t outputPinIndex, uint32_t targetInputPinIndex) {
    if (outputPinIndex < m_OutputPins.size() && targetNode && targetInputPinIndex < targetNode->m_InputPins.size()) {
        m_OutputPins[outputPinIndex].LinkedNode = targetNode;
        m_OutputPins[outputPinIndex].LinkedPinIndex = targetInputPinIndex;
        targetNode->m_InputPins[targetInputPinIndex].LinkedNode = shared_from_this();
        targetNode->m_InputPins[targetInputPinIndex].LinkedPinIndex = outputPinIndex;
        LOG_DEBUG("Nodes connected: {} -> {}", m_NodeID, targetNode->m_NodeID);
    }
}

void BlueprintNode::DisconnectPin(uint32_t pinIndex) {
    if (pinIndex < m_OutputPins.size()) {
        m_OutputPins[pinIndex].LinkedNode = nullptr;
        m_OutputPins[pinIndex].LinkedPinIndex = -1;
    }
}

BlueprintGraph::BlueprintGraph(const std::string& name) : m_Name(name) {
    LOG_INFO("BlueprintGraph created: {}", name);
}

BlueprintGraph::~BlueprintGraph() {
    m_Nodes.clear();
    m_NodeMap.clear();
}

std::shared_ptr<BlueprintNode> BlueprintGraph::AddNode(BlueprintNode::NodeType type, const std::string& name) {
    auto node = std::make_shared<BlueprintNode>(type, name);
    m_Nodes.push_back(node);
    m_NodeMap[node->GetNodeID()] = node;
    return node;
}

void BlueprintGraph::RemoveNode(uint32_t nodeID) {
    auto it = m_NodeMap.find(nodeID);
    if (it != m_NodeMap.end()) {
        auto node = it->second;
        m_Nodes.erase(std::remove(m_Nodes.begin(), m_Nodes.end(), node), m_Nodes.end());
        m_NodeMap.erase(it);
    }
}

std::shared_ptr<BlueprintNode> BlueprintGraph::FindNode(uint32_t nodeID) const {
    auto it = m_NodeMap.find(nodeID);
    return it != m_NodeMap.end() ? it->second : nullptr;
}

void BlueprintGraph::ExecuteGraph() {
    for (auto& node : m_Nodes) {
        node->Execute();
    }
}

void BlueprintGraph::Compile() {
    LOG_DEBUG("Compiling blueprint: {}", m_Name);
    m_IsCompiled = true;
}

} // namespace Luminary
