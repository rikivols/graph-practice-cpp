#ifndef __EVALUATION__
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

#define NO_EMPLOYEE (-1)

#endif

using namespace std;

template <typename T>
void printVectorPair(const vector<pair<T, T>> &vec) {
    cout << "[";
    for (auto v: vec) {
        cout << v.first << " " << v.second << ",  ";
    }
    cout << "]" << endl;
}

template <typename T>
void printVector(const vector<T> &vec) {
    cout << "[";
    for (auto v: vec) {
        cout << v << ", ";
    }
    cout << "]" << endl;
}

template <typename T>
void printMap(const map<T, vector<T>> &mp) {
    cout << "{";
    for (auto [k, vec]: mp) {
        cout << k << ": [";
        for (auto v: vec) {
            cout << v << ", ";
        }
        cout << "]" << endl;
    }
    cout << "}" << endl;
}


// Employee structure
struct Employee {
    vector<int> subordinates;
    vector<int> minCost;
};

// Function to build the tree from the employee vector
void buildTree(vector<Employee>& employees, const vector<int>& bosses) {
    for (int i = 0; i < bosses.size(); ++i) {
        if (bosses[i] != NO_EMPLOYEE) {
            employees[bosses[i]].subordinates.push_back(i);
        }
    }
}

// Recursive function to calculate the minimum cost for each employee
int minCostForEmployee(vector<Employee>& employees, int empId, int lastGift, const vector<int>& giftCosts) {
    if (empId == NO_EMPLOYEE) return 0;

    // Use memoization to avoid recalculating
    if (employees[empId].minCost[lastGift] != -1) {
        return employees[empId].minCost[lastGift];
    }

    int minCost = INT_MAX;
    for (int i = 0; i < giftCosts.size(); ++i) {
        if (i == lastGift) continue; // Skip if same gift as boss

        int cost = giftCosts[i];
        for (int subId : employees[empId].subordinates) {
            cost += minCostForEmployee(employees, subId, i, giftCosts);
        }
        minCost = min(minCost, cost);
    }

    employees[empId].minCost[lastGift] = minCost;
    return minCost;
}

// Main function to find the minimum cost of gifts distribution
int findMinCost(const vector<int>& bosses, const vector<int>& giftCosts) {
    int n = bosses.size();
    vector<Employee> employees(n);
    buildTree(employees, bosses);

    // Initialize minCost with -1 for memoization
    for (int i = 0; i < n; ++i) {
        employees[i].minCost.resize(giftCosts.size(), -1);
    }

    int totalMinCost = 0;
    for (int i = 0; i < n; ++i) {
        if (bosses[i] == NO_EMPLOYEE) { // Start with top-level bosses
            totalMinCost += minCostForEmployee(employees, i, -1, giftCosts);
        }
    }

    return totalMinCost;
}


//
const std::tuple<int, std::vector<int>, vector<int>> EXAMPLES[] = {
  { 17, {1,2,3,4,NO_EMPLOYEE}, {25,4,18,3} },
  { 16, {4,4,4,4,NO_EMPLOYEE}, {25,4,18,3} },
  { 17, {4,4,3,4,NO_EMPLOYEE}, {25,4,18,3} },
  { 24, {4,4,3,4,NO_EMPLOYEE,3,3}, {25,4,18,3} },
};


int main() {
  vector<int> bossesInput1 = {1,2,3,4,NO_EMPLOYEE};
  vector<int> giftsInput1 = {25,4,18,3};

  int minCost = findMinCost(bossesInput1, giftsInput1);
  cout << minCost << endl;

  for (auto example: EXAMPLES) {
      int realResult = findMinCost(get<1>(example), get<2>(example));

      cout << "Expected: " << get<0>(example) << " Real: " << realResult << endl;

  }
}
