#include <iostream>
#include <sstream>
#include <vector>
#include <numeric>

struct ChildRef;
using NodeID = size_t;
using Node = std::vector<ChildRef>;
using Graph = std::vector<Node>;
using Edges = std::vector<std::pair<NodeID, NodeID>>;
using Requests = std::vector<size_t>;

struct ChildRef {
    NodeID nodeID {};
    mutable size_t childCount {};
    size_t countNodes(const Graph&, NodeID parentID) const;
};

size_t countNodes(const Graph& graph, NodeID nodeID, NodeID fromID) {
    size_t count = 1;
    for (const ChildRef& child : graph[nodeID]) {
        if (child.nodeID != fromID)
            count += child.countNodes(graph, nodeID);
    }
    return count;
}

size_t ChildRef::countNodes(const Graph& graph, NodeID parentID) const {
    if (!childCount)
        childCount = ::countNodes(graph, nodeID, parentID);
    return childCount;
}

int main() {

#if 10
    std::istream& input = std::cin;
#else
    std::istringstream input("3 2 1 2 2 3 1 1");
#endif
    size_t n{}, m{};

    input >> n >> m;

    Graph graph(n + 1);
    Edges edges(m + 1);

    for (size_t i = 1; i <= m; ++i) {
        NodeID from{}, to{};
        input >> from >> to;
        graph[from].push_back({to});
        graph[to].push_back({from});
        edges[i] = {from, to};
    }

    size_t q{};

    input >> q;

    Requests requests(q);

    for (size_t i = 0; i < q; ++i) {
        input >> requests[i];
    }

    for (size_t edge : requests) {
        const NodeID first = edges[edge].first;
        const NodeID second = edges[edge].second;
        const auto n1 = countNodes(graph, first, second);
        const auto n2 = countNodes(graph, second, first);
        std::cout << n1 * n2 << " ";
    }
}
