#include "MaterialNode.hpp"
#include "../Core/Logging/Logger.hpp"

namespace Luminary {

uint32_t MaterialNode::s_NodeIDCounter = 0;

MaterialNode::MaterialNode(NodeType type, const std::string& name)
    : m_NodeID(s_NodeIDCounter++), m_Type(type), m_Name(name) {
    LOG_DEBUG("MaterialNode created: {} (ID: {})", name, m_NodeID);
}

MaterialNode::~MaterialNode() = default;

void MaterialNode::AddInputConnection(std::shared_ptr<MaterialNode> sourceNode, uint32_t sourceOutput, uint32_t targetInput) {
    if (sourceNode) {
        m_InputConnections.push_back({sourceNode, sourceOutput, targetInput});
    }
}

void MaterialNode::AddOutputConnection(std::shared_ptr<MaterialNode> targetNode, uint32_t sourceOutput, uint32_t targetInput) {
    if (targetNode) {
        m_OutputConnections.push_back({targetNode, sourceOutput, targetInput});
    }
}

MaterialGraph::MaterialGraph(const std::string& name) : m_Name(name) {
    LOG_INFO("MaterialGraph created: {}", name);
}

MaterialGraph::~MaterialGraph() {
    m_Nodes.clear();
    m_NodeMap.clear();
}

std::shared_ptr<MaterialNode> MaterialGraph::AddNode(MaterialNode::NodeType type, const std::string& name) {
    auto node = std::make_shared<MaterialNode>(type, name);
    m_Nodes.push_back(node);
    m_NodeMap[node->GetNodeID()] = node;
    return node;
}

void MaterialGraph::RemoveNode(uint32_t nodeID) {
    auto it = m_NodeMap.find(nodeID);
    if (it != m_NodeMap.end()) {
        auto node = it->second;
        m_Nodes.erase(std::remove(m_Nodes.begin(), m_Nodes.end(), node), m_Nodes.end());
        m_NodeMap.erase(it);
    }
}

std::shared_ptr<MaterialNode> MaterialGraph::FindNode(uint32_t nodeID) const {
    auto it = m_NodeMap.find(nodeID);
    return it != m_NodeMap.end() ? it->second : nullptr;
}

void MaterialGraph::Evaluate() {
    for (auto& node : m_Nodes) {
        node->Evaluate();
    }
}

void MaterialGraph::Compile() {
    LOG_DEBUG("Compiling material: {}", m_Name);
    m_IsCompiled = true;
}

bool MaterialGraph::Save(const std::string& path) {
    LOG_DEBUG("Saving material graph: {}", path);
    return true;
}

bool MaterialGraph::Load(const std::string& path) {
    LOG_DEBUG("Loading material graph: {}", path);
    return true;
}

} // namespace Luminary
