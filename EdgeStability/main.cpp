#include <iostream>
#include <sstream>
#include <vector>

namespace {

using NodeID = size_t;
using EdgeID = size_t;
using LinkID = size_t;

LinkID getReverseLink(LinkID linkID) { return linkID ^ 1; }
LinkID getFirstLink(EdgeID edgeID) { return edgeID << 1; }
LinkID getSecondLink(EdgeID edgeID) { return getReverseLink(getFirstLink(edgeID)); }
EdgeID getEdge(LinkID linkID) { return linkID >> 1; }

class Graph {
public:

    Graph(size_t nodeCount, size_t edgeCount)
        : m_nodes(nodeCount + 1)
        , m_links((edgeCount + 1) * 2) {
    }

    bool IsConnected() const { return GetEdgeCount() == (GetNodeCount() - 1); }
    size_t GetNodeCount() const { return m_nodes.size() - 1; }
    size_t GetEdgeCount() const { return m_links.size() / 2 - 1; }
    NodeID GetTargetNode(LinkID linkID) const { return m_links[linkID].targetNodeID; }
    size_t GetAdjacentCount(NodeID nodeID) const { return m_nodes[nodeID].size(); }

    void AddEdge(EdgeID edgeID, NodeID firstNodeID, NodeID secondNodeID) {
        m_nodes[firstNodeID].push_back(getFirstLink(edgeID));
        m_nodes[secondNodeID].push_back(getSecondLink(edgeID));
        m_links[getFirstLink(edgeID)].targetNodeID = secondNodeID;
        m_links[getSecondLink(edgeID)].targetNodeID = firstNodeID;
    }

    size_t CountReachableNodes(LinkID linkID) {
        auto& link = m_links[linkID];
        if (!link.reachableCount) {
            link.reachableCount = 1;
            const auto& node = m_nodes[link.targetNodeID];
            for (const LinkID childLinkID : node) {
                if (childLinkID != getReverseLink(linkID)) {
                    link.reachableCount += CountReachableNodes(childLinkID);
                }
            }
        }
        return link.reachableCount;
    }

private:
    using Node = std::vector<LinkID>;
    struct Link {
        NodeID targetNodeID{};
        size_t reachableCount{};
    };
    std::vector<Node> m_nodes;
    std::vector<Link> m_links;
};

} // namespace

int main() {

#if 10
    std::istream& input = std::cin;
#else
    std::istringstream input("3 2 1 2 2 3 1 1");
#endif
    size_t n{}, m{};

    input >> n >> m;

    Graph graph(n, m);

    for (EdgeID edgeID = 1; edgeID <= m; ++edgeID) {
        NodeID from{}, to{};
        input >> from >> to;
        graph.AddEdge(edgeID, from, to);
    }

    size_t q{};
    input >> q;

    if (graph.IsConnected()) {
        for (size_t i = 0; i < q; ++ i) {
            EdgeID edgeID {};
            input >> edgeID;
            const LinkID link1 = getFirstLink(edgeID);
            const LinkID link2 = getSecondLink(edgeID);
            const size_t adjCount1 = graph.GetAdjacentCount(graph.GetTargetNode(link1));
            const size_t adjCount2 = graph.GetAdjacentCount(graph.GetTargetNode(link2));
            const size_t reachCount1 = adjCount1 < adjCount2 ?
                graph.CountReachableNodes(link1) :
                graph.CountReachableNodes(link2);
            const size_t reachCount2 = n - reachCount1;
            const size_t pathCount = reachCount1 * reachCount2;
            std::cout << " " << pathCount;
        }
    }
    else {
        for (size_t i = 0; i < q; ++i) {
            EdgeID edgeID{};
            input >> edgeID;
            const size_t reachCount1 = graph.CountReachableNodes(getFirstLink(edgeID));
            const size_t reachCount2 = graph.CountReachableNodes(getSecondLink(edgeID));
            const size_t pathCount = reachCount1 * reachCount2;
            std::cout << " " << pathCount;
        }
    }
}
