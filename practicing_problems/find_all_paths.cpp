#include <cassert>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <random>
#include <type_traits>


using namespace std;


int findAllPathsRec(vector<vector<int>> &graph, vector<int> &memorizedTable, int currentIndex=0) {
    if (graph[currentIndex].empty()) {
        return 0;
    }

    if (memorizedTable[currentIndex] != -1) {
        return memorizedTable[currentIndex];
    }

    int answer;

    if (graph[currentIndex].size() == 1) {
        answer = max(1, findAllPathsRec(graph, memorizedTable, graph[currentIndex][0]));
    }
    else {
        answer = findAllPathsRec(graph, memorizedTable, graph[currentIndex][0]) +
                findAllPathsRec(graph, memorizedTable, graph[currentIndex][1]);
    }
    memorizedTable[currentIndex] = answer;
    return answer;
}


int findAllPaths(vector<vector<int>> &graph) {
    vector<int> memorizedTable = {};

    for (size_t i=0; i<graph.size(); i++) {
        memorizedTable.push_back(-1);
    }

    return findAllPathsRec(graph, memorizedTable);
}


int main() {
    vector<vector<int>> input = {{1, 2}, {3}, {3}, {4}, {}};
    int allPaths = findAllPaths(input);

    cout << allPaths;
}
