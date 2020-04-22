#include <iostream>
#include <vector>
#include <algorithm>
#include <set>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

size_t n(0), m(0);
vector<vector<int>> g;
vector<int> buf;
std::set<int> set;
vector<bool> passed;

bool dfs(int v0) {

    if (!passed[v0])
    {
        const auto insertion = set.insert(v0);
        if (!insertion.second)
        {
            const auto cyclePoint = std::find(buf.begin(), buf.end(), v0);
            buf.erase(buf.begin(), cyclePoint);
            passed[v0] = true;
            return true;
        }

        buf.push_back(v0);

        for (const int v1 : g[v0]) {
            if (dfs(v1)) {
                passed[v0] = true;
                return true;
            }
        }
        buf.pop_back();
        set.erase(insertion.first);
        passed[v0] = true;
    }
    return false;
}


int main() {
    cin >> n >> m;
    g = vector<vector<int>>(n + 1);
    passed = vector<bool>(n + 1, false);

    for (size_t i = 0; i < m; ++i) {
        size_t from, to;
        cin >> from >> to;
        g[from].push_back(to);
    }

    bool cycleFound = false;
    for (size_t v = 1; v <= n; ++v) {
        if (dfs(v)) {
            cycleFound = true;
            break;
        }
    }
    if (!cycleFound)
        cout << "NO" << endl;
    else {
        cout << "YES" << endl;
        for (auto e : buf)
            cout << e << " ";
    }
}
