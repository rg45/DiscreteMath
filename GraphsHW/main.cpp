#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

size_t n(0), m(0); 
int v(0);
vector<vector<int>> g;
vector<int> buf;

void bfs(const vector<int>& front) {
    if (!front.empty()) {
        std::vector<int> children;
        for (const int v0 : front) {
            if (std::find(buf.begin(), buf.end(), v0) == buf.end())
            {
                buf.push_back(v0);
                for (const int v1 : g[v0])
                    children.push_back(v1);
            }
        }
        bfs(children);
    }
}

int main() {
    cin >> n >> m >> v;
    g = vector<vector<int>>(n + 1);

    for (size_t i = 0; i < m; ++i) {
        size_t from, to;
        cin >> from >> to;
        g[from].push_back(to);
        g[to].push_back(from);
    }

    bfs({v});

    cout << buf.size() << endl;
    for (auto e : buf)
        cout << e << " ";
}
