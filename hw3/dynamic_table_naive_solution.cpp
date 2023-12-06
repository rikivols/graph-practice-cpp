#ifndef __PROGTEST__
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

using Price = unsigned long long;
using Employee = size_t;
inline constexpr Employee NO_EMPLOYEE = -1;
using Gift = size_t;

#endif

using namespace std;


//template <typename T>
//void printVector(const vector<T> &vec) {
//    cout << "[";
//    for (auto v: vec) {
//        cout << v << ", ";
//    }
//    cout << "]" << endl;
//}
//
//template <typename T>
//void printDoubleVector(const vector<vector<T>> &vec) {
//    cout << "[" << endl;
//    for (auto ve: vec) {
//        cout << "  ";
//        printVector(ve);
//    }
//    cout << "]" << endl;
//}
//
//
//template <typename T>
//void printVectorPair(const vector<pair<T, T>> &vec) {
//    cout << "[";
//    for (auto v: vec) {
//        cout << v.first << " " << v.second << ",  ";
//    }
//    cout << "]" << endl;
//}


class Employee_relations {
public:
    Employee_relations(): boss_depth(0) {}

    void add_underling(Employee employee) {
        underlings.push_back(employee);
    }

    [[nodiscard]] bool is_lowest_employee() const {
        return underlings.empty();
    }

    [[nodiscard]] bool is_highest_boss() const {
        return boss == NO_EMPLOYEE;
    }

    vector<Employee> underlings;
    size_t boss_depth;
    Employee boss = NO_EMPLOYEE;
};


class Comparison {
    const vector<Employee_relations> &employees;

public:
    explicit Comparison(const vector<Employee_relations> &employees): employees(employees) {}

    bool operator()(Employee emp1, Employee emp2) const {
        return employees[emp1].boss_depth > employees[emp2].boss_depth;
    }
};


class Employee_database {
public:
    explicit Employee_database(const vector<Employee>& boss): employees(boss.size()), dynamic_table(boss.size()) {
        for (size_t i=0; i < boss.size(); i++) {
            Employee employee_boss = boss[i];
            if (employee_boss != NO_EMPLOYEE) {
                employees[employee_boss].add_underling(i);
                employees[i].boss = employee_boss;
            }
        }

        dynamic_table.resize(boss.size());
    }

    void calculate_boss_depth_from_employee(Employee cur_employee, size_t current_boss_depth) {
        employees[cur_employee].boss_depth = current_boss_depth;

        for (auto employee_underling: employees[cur_employee].underlings) {
            // calculate the depth of the boss's subordinate
            calculate_boss_depth_from_employee(employee_underling, 1 + current_boss_depth);
        }
    }

    /*
     * Calculates how many levels of bosses the employee has. Could be understood as a height of the employee
     * tree.
     *
     * depth of 3 means that the employee has a boss, which also has a boss, which also has the highest boss
     */
    void calculate_all_boss_depths() {

        for (size_t i=0; i < employees.size(); i++) {
            // run only from the highest bosses to avoid overriding the depths
            if (employees[i].is_highest_boss()) {
                calculate_boss_depth_from_employee(i, i);
            }
        }
    }

    /*
     * Returns employee names sorted by their boss depth in the descending order
     * (gets employees with no underlings first)
     */
    void sort_by_boss_depth() {
        employees_sorted_by_boss_depth.reserve(employees.size());
        for (size_t i=0; i<employees.size(); i++) {
            employees_sorted_by_boss_depth.push_back(i);
        }
        Comparison comparison_function(employees);
        sort(employees_sorted_by_boss_depth.begin(), employees_sorted_by_boss_depth.end(), comparison_function);
    }

    /*
     * Calculates what's the minimal cost of gifts for the sum of gifts of the employee (if they picked j-th gift)
     * and all of its underlings (and their underlings...)
     *
     * We're using the dynamic table's values of the previously calculated values of the underlings.
     */
    Price calculate_minimal_cost_for_employee_subtree(size_t i, size_t j, Price gift_price,
                                                      const vector<pair<Price, Gift>> &gift_prices) {
        Price min_price_of_gifts = gift_price;

        for (Employee employee_underling: employees[i].underlings) {
            Price min_price_for_underling = ULLONG_MAX;
            for (auto [_, gift_index]: gift_prices) {
                // we have to ban this option, employee's boss can't have the same gift as its underling
                if (j == gift_index) {
                    continue;
                }

                Price stored_gift_price = dynamic_table[employee_underling][gift_index];

                // we found a better option for gift price for the underling
                if (stored_gift_price < min_price_for_underling) {
                    min_price_for_underling = stored_gift_price;
                }
            }
            if (min_price_for_underling != ULLONG_MAX) {
                min_price_of_gifts += min_price_for_underling;
            }
        }

        return min_price_of_gifts;
    }

    /*
     * Fills cell of the dynamic table representing gift price for the given employee with the
     * minimal price of the employee subtree
     */
    void fill_dynamic_table_cell(size_t i, size_t j, Price gift_price, const vector<pair<Price, Gift>> &gift_prices) {

        if (employees[i].is_lowest_employee()) {
            dynamic_table[i][j] = gift_price;
            return;
        }

        Price min_price_for_employee = calculate_minimal_cost_for_employee_subtree(i, j, gift_price, gift_prices);
        dynamic_table[i][j] = min_price_for_employee;
    }

    void populate_gifts_table(const vector<pair<Price, Gift>> &gift_prices) {
        for (auto &vec : dynamic_table) {
            vec.resize(gift_prices.size(), ULLONG_MAX);
        }
        // row of the table, representing the employee
        for (size_t i: employees_sorted_by_boss_depth) {
            // column of the table, representing the gift
            for (auto [gift_price, gift_index]: gift_prices) {
                fill_dynamic_table_cell(i, gift_index, gift_price, gift_prices);
            }
        }

//        printDoubleVector(dynamic_table);
    }

    Gift find_cheapest_gift(Employee employee_index, Gift last_gift) {
        Price min_price = ULLONG_MAX;
        Gift cheapest_gift = 0;
        for (size_t i=0; i<dynamic_table[employee_index].size(); i++) {
            if (last_gift != i and dynamic_table[employee_index][i] < min_price) {
                min_price = dynamic_table[employee_index][i];
                cheapest_gift = i;
            }
        }

        return cheapest_gift;
    }

    void depth_first_find_gifts(Employee employee_index, vector<Gift> &picked_gifts) {
        stack<Employee> next_employees;
        next_employees.push(employee_index);

        while (!next_employees.empty()) {
            Employee current_employee = next_employees.top();
            next_employees.pop();

            // find what the gift was for the boss, so we don't give the same one to the underling
            Gift boss_gift = UINT_MAX;
            if (!employees[current_employee].is_highest_boss()) {
                boss_gift = picked_gifts[employees[current_employee].boss];
            }

            Gift cheapest_gift = find_cheapest_gift(current_employee, boss_gift);

            picked_gifts[current_employee] = cheapest_gift;

            for (Employee underling: employees[current_employee].underlings) {
                next_employees.push(underling);
            }
        }
    }

    void pick_gifts(pair<Price, vector<Gift>> &picked_gifts) {
        picked_gifts.first = 0;
        picked_gifts.second.resize(employees.size(), 0);

        for (size_t i=0; i < employees.size(); i++) {
            if (employees[i].is_highest_boss()) {
                depth_first_find_gifts(i, picked_gifts.second);
                picked_gifts.first += *min_element(dynamic_table[i].begin(), dynamic_table[i].end());
            }
        }
    }

    /*
     * Now our result contains gifts that are sorted by their price. We need to restore their previous indexes
     */
    void recalculate_picked_gifts(pair<Price, vector<Gift>> &picked_gifts,
                                  vector<pair<Price, Gift>> sorted_prices) {
        for (Gift &i: picked_gifts.second) {
            i = sorted_prices[i].second;
        }
    }

private:
    vector<Employee_relations> employees;
    vector<Employee> employees_sorted_by_boss_depth;
    vector<vector<Employee>> dynamic_table;
};


pair<Price, vector<Gift>> optimize_gifts(const vector<Employee>& boss, const vector<Price>& gift_prices) {

    Employee_database employee_database(boss);
    employee_database.calculate_all_boss_depths();

    employee_database.sort_by_boss_depth();

    vector<pair<Price, Gift>> sorted_prices;
    for (size_t i=0; i < gift_prices.size(); i++) {
        sorted_prices.emplace_back(gift_prices[i], i);
    }
    sort(sorted_prices.begin(), sorted_prices.end());

    if (gift_prices.size() >= boss.size()) {
        vector<pair<Price, Gift>> temp(sorted_prices.begin(), sorted_prices.begin() + (long) boss.size());
        sorted_prices = temp;
    }

    employee_database.populate_gifts_table(sorted_prices);

    pair<Price, vector<Gift>> picked_gifts;
    employee_database.pick_gifts(picked_gifts);

//    employee_database.recalculate_picked_gifts(picked_gifts, sorted_prices);

//    printVector(picked_gifts.second);

    return picked_gifts;
}

#ifndef __PROGTEST__

const std::tuple<Price, std::vector<Employee>, std::vector<Price>> EXAMPLES[] = {
    { 17, {1,2,3,4,NO_EMPLOYEE}, {25,4,18,3} },
    { 16, {4,4,4,4,NO_EMPLOYEE}, {25,4,18,3} },
    { 17, {4,4,3,4,NO_EMPLOYEE}, {25,4,18,3} },
    { 24, {4,4,3,4,NO_EMPLOYEE,3,3}, {25,4,18,3} },
    { 18, {1,4,5,1,NO_EMPLOYEE,4,2,NO_EMPLOYEE,NO_EMPLOYEE}, {4, 8, 23, 99, 1, 8}},
    { 23, {1,4,5,1,NO_EMPLOYEE,4,2,1,4,4}, {4, 8, 23, 99, 1, 8}}
};

#define CHECK(cond, ...) do { \
    if (cond) break; \
    printf("Test failed: " __VA_ARGS__); \
    printf("\n"); \
    return false; \
  } while (0)

bool test(Price p, const std::vector<Employee>& boss, const std::vector<Price>& gp) {
  auto&& [ sol_p, sol_g ] = optimize_gifts(boss, gp);
  CHECK(sol_g.size() == boss.size(),
    "Size of the solution: expected %zu but got %zu.", boss.size(), sol_g.size());

  Price real_p = 0;
  for (Gift g : sol_g) real_p += gp[g];
  CHECK(real_p == sol_p, "Sum of gift prices is %llu but reported price is %llu.", real_p, sol_p);

  if (0) {
    for (Employee e = 0; e < boss.size(); e++) printf(" (%zu)%zu", e, sol_g[e]);
    printf("\n");
  }

  for (Employee e = 0; e < boss.size(); e++)
    CHECK(boss[e] == NO_EMPLOYEE || sol_g[boss[e]] != sol_g[e],
      "Employee %zu and their boss %zu has same gift %zu.", e, boss[e], sol_g[e]);

  CHECK(p == sol_p, "Wrong price: expected %llu got %llu.", p, sol_p);

  return true;
}
#undef CHECK

int main() {
  int ok = 0, fail = 0;
  for (auto&& [ p, b, gp ] : EXAMPLES) (test(p, b, gp) ? ok : fail)++;
  
  if (!fail) printf("Passed all %d tests!\n", ok);
  else printf("Failed %d of %d tests.", fail, fail + ok);
}

#endif


