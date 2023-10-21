#ifndef __PROGTEST__
#include <algorithm>
#include <array>
#include <bitset>
#include <cassert>
#include <cstdint>
#include <deque>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <optional>
#include <queue>
#include <random>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <vector>

enum Point : size_t {};

struct Path {
  Point from, to;
  unsigned length;

  Path(size_t f, size_t t, unsigned l) : from{f}, to{t}, length{l} {}

  friend bool operator == (const Path& a, const Path& b) {
    return std::tie(a.from, a.to, a.length) == std::tie(b.from, b.to, b.length);
  }
  
  friend bool operator != (const Path& a, const Path& b) { return !(a == b); }
};

#endif

std::ostream& operator << (std::ostream& out, const Path& p) {
    out << "from: " << p.from << " to: " << p.to << " length: " << p.length;
    return out;
}

using namespace std;

template <typename T>
void printVector(std::vector<T> vecToPrint) {
    std::cout << "{ ";
    for (size_t i=0; i<vecToPrint.size(); i++) {
        std::cout << vecToPrint[i] << ", ";
    }
    std::cout<< "}" << std::endl;
}


void topsort(const vector<vector<Path>> &graphPaths, std::vector<Point> &topologicalSort,
             vector<long> &distances, vector<Path> &previousPaths) {
    /*
     * Generate topsort order. Fill distances with negative infinity and first nodes in top. order with zero.
     */

    queue<Point> nextVertex;
    size_t vertexNum = graphPaths.size();

    // we fill our outputCounter with the number all edges that lead to that vertex
    vector<size_t> outputCounter(vertexNum, 0);

    for (size_t i=0; i<vertexNum; i++) {
        outputCounter[i] = 0;
    }

    for (const auto& pathGroup: graphPaths) {
        for (Path vertexPath: pathGroup) {
            outputCounter[vertexPath.to]++;
        }
    }

    distances.resize(vertexNum);
    previousPaths.reserve(vertexNum);

    // all vertexes that don't have an edge leading to it are pushed to queue (they're the first in topological order)
    for (size_t i=0; i<vertexNum; i++) {
        if (outputCounter[i] == 0) {
            nextVertex.push(Point(i));
            distances[i] = 0;
        }
        else {
            distances[i] = INT_MIN;
        }
        previousPaths.emplace_back(0, 0, 0);
    }

    // main loop, find out if there's a cycle or not
    while (!nextVertex.empty()) {
        Point curVertex = nextVertex.front();
        nextVertex.pop();

        topologicalSort.push_back(curVertex);

        for (Path pathGroup: graphPaths[curVertex]) {
            outputCounter[pathGroup.to]--;
            if (outputCounter[pathGroup.to] == 0) {
                nextVertex.push(pathGroup.to);
            }
        }
    }
}


void convertPathToGraph(const std::vector<Path>& all_paths, size_t maxVertices, vector<vector<Path>> &graphPaths) {
    /*
     * Converts paths (Path1, Path2...) to a graph, where each path is stored at a vector's position from
     * which it originates (from).
     *
     * { {PATH1, PATH2}, {PATH3}, {}, ... }
     */

    for (size_t i=0; i<maxVertices; i++) {
        graphPaths.emplace_back();
    }

    for (auto path: all_paths) {
        graphPaths[path.from].push_back(path);
    }
}


void storeDistances(const vector<Point> &topologicalSort, const vector<vector<Path>> &graphPaths,
                    vector<long> &distances, vector<Path> &previousPaths) {
    /*
     * Pre-calculate longest distance for each vertex (how much it takes to get to it)
     */

    for (Point vertex: topologicalSort) {
        for (Path vertexPath: graphPaths[vertex]) {
            if (distances[vertex] + vertexPath.length > distances[vertexPath.to]) {
                distances[vertexPath.to] = distances[vertex] + vertexPath.length;
                previousPaths[vertexPath.to] = vertexPath;
            }
        }
    }
}


void reconstructLongestPath(const vector<long> &distances, const vector<Path> &previousPaths,
                            vector<Path> &finalResult) {
    /*
     * Reconstruct the longest path from the pre-calculated distances and stored longest paths
     * for each vertex.
     */

    long maxDistance = -1000;
    int maxDistanceIndex = -1;

    for (int i=0; i<(int)distances.size(); i++) {
        if (distances[i] > maxDistance) {
            maxDistance = distances[i];
            maxDistanceIndex = i;
        }
    }

    int currentIndex = maxDistanceIndex;
    while (true) {
        if (distances[currentIndex] == 0) {
            break;
        }
        finalResult.push_back(previousPaths[currentIndex]);
        currentIndex = previousPaths[currentIndex].from;
    }

    reverse(finalResult.begin(), finalResult.end());
}


std::vector<Path> longest_track(size_t points, const std::vector<Path>& all_paths) {
    vector<vector<Path>> graphPaths;
    vector<Point> topologicalSort;

    convertPathToGraph(all_paths, points, graphPaths);
    vector<long> distances;
    vector<Path> previousPaths;

    topsort(graphPaths, topologicalSort, distances, previousPaths);

    storeDistances(topologicalSort, graphPaths, distances, previousPaths);

    vector<Path> finalResult;

    reconstructLongestPath(distances, previousPaths, finalResult);

    return finalResult;
};


#ifndef __PROGTEST__


struct Test {
  unsigned longest_track;
  size_t points;
  std::vector<Path> all_paths;
};

inline const Test TESTS[] = {
  {13, 5, { {3,2,10}, {3,0,9}, {0,2,3}, {2,4,1} } },
  {11, 5, { {3,2,10}, {3,1,4}, {1,2,3}, {2,4,1} } },
  {16, 8, { {3,2,10}, {3,1,1}, {1,2,3}, {1,4,15} } },
};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Fail: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool run_test(const Test& t) {
  auto sol = longest_track(t.points, t.all_paths);

  unsigned length = 0;
  for (auto [ _, __, l ] : sol) length += l;

  CHECK(t.longest_track == length,
    "Wrong length: got %u but expected %u", length, t.longest_track);

  for (size_t i = 0; i < sol.size(); i++) {
    CHECK(std::count(t.all_paths.begin(), t.all_paths.end(), sol[i]),
      "Solution contains non-existent path: %zu -> %zu (%u)",
      sol[i].from, sol[i].to, sol[i].length);

    if (i > 0) CHECK(sol[i].from == sol[i-1].to,
      "Paths are not consecutive: %zu != %zu", sol[i-1].to, sol[i].from);
  }

  return true;
}
#undef CHECK

int main() {
  int ok = 0, fail = 0;

  for (auto&& t : TESTS) (run_test(t) ? ok : fail)++;
  
  if (!fail) printf("Passed all %i tests!\n", ok);
  else printf("Failed %u of %u tests.\n", fail, fail + ok);
}

#endif


