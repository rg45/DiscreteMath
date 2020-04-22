#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>

using TownNumber = size_t;
using Node = std::set<TownNumber>;
using Graph = std::vector<Node>;
using Leaves = std::set<TownNumber>;

bool isLeave(const Node& node) {
    return node.size() == 1;
}

Leaves colllectLeaves(const Graph& graph) {
    Leaves leaves;
    for (TownNumber i = 1; i < graph.size(); ++i) {
        if (isLeave(graph[i]))
            leaves.insert(i);
    }
    return leaves;
}

Leaves dropLeaves(Graph& graph, const Leaves& input) {
    Leaves output;
    for (const TownNumber currN : input) {
        Node& curr = graph[currN];
        if (!curr.empty()) {
            const TownNumber nextN = *curr.begin();
            Node& next = graph[nextN];
            next.erase(currN);
            if (isLeave(next))
                output.insert(nextN);
        }
    }
    return output;
}

int main() {

    // WTF?
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);
    std::cout.tie(NULL);

#if 10
    std::istream& input = std::cin;
#else
    std::istringstream input("3 1 2 3");
#endif

    Graph graph;
    TownNumber townCount{};

    input >> townCount;

    if (townCount == 1) {
        std::cout << "0 1 1";
        return 0;
    }

    graph.resize(townCount + 1);

    for (TownNumber i = 0; i < townCount - 1; ++i) {
        int from{}, to{};
        input >> from >> to;
        graph[from].insert(to);
        graph[to].insert(from);
    }

    size_t radius = 0;
    Leaves center = colllectLeaves(graph);

    for (;;)
    {
        Leaves leaves = dropLeaves(graph, center);
        if (!leaves.empty()) {
            std::swap(leaves, center);
            ++radius;
        }
        else {
            break;
        }
    }
    if (center.size() == 2)
        ++radius;

    std::cout << radius << " " << center.size();
    for (TownNumber n : center)
        std::cout << " " << n;
}
