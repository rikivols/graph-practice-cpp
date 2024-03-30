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
void printDoubleVector(const vector<vector<T>> &vec) {
    cout << "[" << endl;
    for (auto ve: vec) {
        cout << "  ";
        printVector(ve);
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


using namespace std;


struct Employee {
    vector<int> subordinates;
    int indegree = 0; // For topological sorting
};

// Function to build the tree and calculate indegrees for topological sort
void buildTreeAndCalculateIndegrees(vector<Employee>& employees, const vector<int>& bosses) {
    for (int i = 0; i < bosses.size(); ++i) {
        if (bosses[i] != NO_EMPLOYEE) {
            employees[bosses[i]].subordinates.push_back(i);
            employees[i].indegree++;
        }
    }
}

// Topological sort to process employees in the correct order
vector<int> topologicalSort(vector<Employee>& employees) {
    int n = employees.size();
    queue<int> q;
    for (int i = 0; i < n; ++i) {
        if (employees[i].indegree == 0) {
            q.push(i);
        }
    }

    vector<int> order;
    while (!q.empty()) {
        int empId = q.front();
        q.pop();
        order.push_back(empId);

        for (int subId : employees[empId].subordinates) {
            if (--employees[subId].indegree == 0) {
                q.push(subId);
            }
        }
    }
    return order;
}

struct Giftt {
    int id;
    int price;
};

int findMinCost(const vector<int>& bosses, const vector<int>& giftCosts) {
    int n = bosses.size();
    vector<Employee> employees(n);
    buildTreeAndCalculateIndegrees(employees, bosses);

    vector<int> sortedEmployees = topologicalSort(employees);
    reverse(sortedEmployees.begin(), sortedEmployees.end());

    printVector(sortedEmployees);

    vector<vector<int>> dp(n, vector<int>(giftCosts.size(), INT_MAX));

    vector<pair<Giftt, Giftt>> min_two_employee_costs(n, {{0, INT_MAX}, {0, INT_MAX}});

    // Dynamic programming calculation
    for (int empId : sortedEmployees) {
        for (int j = 0; j < giftCosts.size(); ++j) {
            int cost = giftCosts[j];

            for (int subId : employees[empId].subordinates) {
                if (min_two_employee_costs[subId].first.id == j) {
                    cost += min_two_employee_costs[subId].second.price;
                }
                else {
                    cost += min_two_employee_costs[subId].first.price;
                }
            }
            if (cost < min_two_employee_costs[empId].first.price) {
                min_two_employee_costs[empId].second = min_two_employee_costs[empId].first;
                min_two_employee_costs[empId].first = {j, cost};
            }
            else if (cost < min_two_employee_costs[empId].second.price) {
                min_two_employee_costs[empId].second = {j, cost};
            }

            dp[empId][j] = cost;
        }
    }

    printDoubleVector(dp);

    // Calculate the total minimum cost
    int totalMinCost = 0;
    for (int i = 0; i < n; ++i) {
        if (bosses[i] == NO_EMPLOYEE) {
            totalMinCost += *min_element(dp[i].begin(), dp[i].end());
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
  { 18, {1,4,5,1,NO_EMPLOYEE,4,2,NO_EMPLOYEE,NO_EMPLOYEE}, {4, 7, 23, 99, 1, 7}},
  { 23, {1,4,5,1,NO_EMPLOYEE,4,2,1,4,4}, {4, 8, 23, 99, 1, 8}}
};


int main() {

  for (auto example: EXAMPLES) {
      int realResult = findMinCost(get<1>(example), get<2>(example));

      cout << "Expected: " << get<0>(example) << " Real: " << realResult << endl;

  }


}
