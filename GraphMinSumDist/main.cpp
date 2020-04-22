#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>


struct GraphNode {

    size_t dist = 0;
    size_t nodes = 1;
    std::set<GraphNode*> adjacent;

    bool isLeaf() const { return adjacent.size() <= 1; }
    size_t estimation() const noexcept { return dist + nodes; }
};

struct GraphNodeComparer {
    bool operator()(const GraphNode* lhs, const GraphNode* rhs) const {
        return lhs->estimation() < rhs->estimation();
    }
};

using Graph = std::vector<GraphNode>;
using Front = std::multiset<GraphNode*, GraphNodeComparer>;

Front makeFront(Graph& graph) {
    Front front;
    for (size_t i = 1; i < graph.size(); ++i) {
        if (graph[i].isLeaf())
            front.insert(&graph[i]);
    }
    return front;
}

void moveFront(Front& front) {
    while (!front.empty()) {
        const auto it = front.begin();
        GraphNode& curr = **it;
        if (!curr.adjacent.empty()) {
            GraphNode& next = **curr.adjacent.begin();
            if (!next.isLeaf() || curr.nodes < next.nodes) {
                next.dist += curr.estimation();
                next.nodes += curr.nodes;
                next.adjacent.erase(&curr);
                if (next.isLeaf())
                    front.insert(&next);
            }
            else if (curr.nodes == next.nodes) {
                next.dist += curr.estimation();
                curr.dist = next.dist;
                break;
            }
        }
        else if (front.size() == 1) {
            break;
        }
        front.erase(it);
    }
}

int main() {

#if 10
    std::istream& input = std::cin;
#else
    std::istringstream input("1");
#endif

    Graph graph;
    size_t townCount{};

    input >> townCount;
    graph.resize(townCount + 1);

    for (size_t i = 0; i < townCount - 1; ++i) {
        int from{}, to{};
        input >> from >> to;
        graph[from].adjacent.insert(&graph[to]);
        graph[to].adjacent.insert(&graph[from]);
    }
    Front front = makeFront(graph);
    moveFront(front);

    std::set<size_t> towns;
    for (const GraphNode* node : front) {
        towns.insert(node - &graph[0]);
    }
    std::cout << (*front.begin())->dist << " " << front.size();
    for (const size_t town : towns) {
        std::cout << " " << town;
    }
}
