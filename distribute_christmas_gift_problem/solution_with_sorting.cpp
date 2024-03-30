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


using namespace std;


struct GiftPrice {
    Gift id;
    Price price;
};


class Employee_relations {
public:
    Employee_relations() = default;

    void add_underling(Employee employee) noexcept {
        underlings.push_back(employee);
    }

    [[nodiscard]] bool is_highest_boss() const noexcept {
        return boss == NO_EMPLOYEE;
    }

    vector<Employee> underlings;
    Employee boss = NO_EMPLOYEE;
};


class Employee_database {
public:
    explicit Employee_database(const vector<Employee>& boss) noexcept: employees(boss.size()), dynamic_table(boss.size()),
                                                              cheapest_employee_gifts(boss.size(), {0, ULLONG_MAX}),
                                                              second_cheapest_employee_gifts(boss.size(), {0, ULLONG_MAX})
    {
        for (size_t i=0; i < boss.size(); i++) {
            Employee employee_boss = boss[i];
            if (employee_boss != NO_EMPLOYEE) {
                employees[employee_boss].add_underling(i);
                employees[i].boss = employee_boss;
            }
            else {
                highest_bosses.push_back(i);
            }
        }
    }

    /**
     * Returns employee ids with a topsort order
     */
    void topsort_employees() noexcept {
        queue<Employee> next_vertex;
        size_t vertex_num = employees.size();

        // we fill our output_counter with the number all edges that lead to that vertex
        vector<size_t> output_counter(vertex_num, 0);

        // calculate how many bosses each person has
        for (const Employee_relations &employee_relations: employees) {
            for (Employee neighbour_vertex: employee_relations.underlings) {
                output_counter[neighbour_vertex]++;
            }
        }

        // all vertexes that don't have an edge leading to it are pushed to queue (they're the first in topological order)
        for (size_t i=0; i<vertex_num; i++) {
            if (output_counter[i] == 0) {
                next_vertex.push(i);
            }
        }

        // main loop, add to topological sort if employee has no more bosses
        while (!next_vertex.empty()) {
            Employee curVertex = next_vertex.front();
            next_vertex.pop();

            employees_topsorted.push_back(curVertex);

            for (auto neighbour_vertex: employees[curVertex].underlings) {
                output_counter[neighbour_vertex]--;
                if (output_counter[neighbour_vertex] == 0) {
                    next_vertex.push(neighbour_vertex);
                }
            }
        }

    }

    /**
     * Fills the dynamic table for the pre-sums
     *
     * @param gift_price vector containing all gift prices
     */
    void populate_gifts_table(const vector<pair<Price, Gift>> &sorted_prices) noexcept {
        for (size_t i=0; i < dynamic_table.size(); i++) {
            size_t new_size = employees[i].underlings.size() + 2;
            dynamic_table[i].resize(new_size, ULLONG_MAX);
        }

        reverse(employees_topsorted.begin(), employees_topsorted.end());

        // row of the table, representing the employee, iterate in reverse topsort order, from underlings to bosses
        for (size_t i: employees_topsorted) {
            size_t max_size = employees[i].underlings.size() + 2;
            // column of the table, representing the gift
            for (size_t j=0; j<sorted_prices.size(); j++) {
                Price min_price_for_employee = sorted_prices[j].first;

                for (Employee underling: employees[i].underlings) {
                    // our boss already has that gift, pick the second-cheapest gift
                    if (cheapest_employee_gifts[underling].id == j) {
                        min_price_for_employee += second_cheapest_employee_gifts[underling].price;
                    }
                    else {
                        min_price_for_employee += cheapest_employee_gifts[underling].price;
                    }
                }

                // we found the new best gift for the employee
                if (min_price_for_employee < cheapest_employee_gifts[i].price) {
                    second_cheapest_employee_gifts[i] = cheapest_employee_gifts[i];
                    cheapest_employee_gifts[i] = {j, min_price_for_employee};
                }
                else if (min_price_for_employee < second_cheapest_employee_gifts[i].price) {
                    second_cheapest_employee_gifts[i] = {j, min_price_for_employee};
                }

                dynamic_table[i][j] = min_price_for_employee;

                if (j >= max_size - 1) {
                    break;
                }
            }
        }
    }

    /**
     * Finds the cheapest gift for the employee
     *
     * @param employee_index what employee it is
     * @param last_gift what gift did the employee's boss have
     * @return
     */
    Gift find_cheapest_gift(Employee employee_index, Gift last_gift) noexcept {
        if (cheapest_employee_gifts[employee_index].id == last_gift) {
            return second_cheapest_employee_gifts[employee_index].id;
        }

        return cheapest_employee_gifts[employee_index].id;
    }

    /**
     * Finds the optimal gifts for each employee and all of its subordinates, direct and indirect.
     *
     * @param employee_index the highest boss for our find
     * @param picked_gifts vector that we're filling all employee's picked gifts with, our final result
     * @param gift_prices vector containing all gift prices
     * @return
     */
    Price depth_first_find_gifts(Employee employee_index, vector<Gift> &picked_gifts,
                                 const vector<pair<Price, Gift>> &sorted_gifts) noexcept {
        stack<Employee> next_employees;
        next_employees.push(employee_index);
        Price all_gifts_price = 0;

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
            all_gifts_price += sorted_gifts[cheapest_gift].first;

            for (Employee underling: employees[current_employee].underlings) {
                next_employees.push(underling);
            }
        }

        return all_gifts_price;
    }

    /**
     * Fills the picked_gifts vector with the optimal gifts for each employee. We start with each employee that
     * doesn't have a boss and run depth first search on it.
     *
     * @param picked_gifts the final vector that we fill the picked gifts with
     * @param gift_prices vector containing all gift prices
     */
    void pick_gifts(pair<Price, vector<Gift>> &picked_gifts, const vector<pair<Price, Gift>> &sorted_gifts) noexcept {
        picked_gifts.first = 0;
        picked_gifts.second.resize(employees.size(), 0);

        for (Employee employee_index: highest_bosses) {
            Price price_together = depth_first_find_gifts(employee_index, picked_gifts.second, sorted_gifts);
            picked_gifts.first += price_together;
        }
    }

    /*
     * Now our result contains gifts that are sorted by their price. We need to restore their previous indexes
     */
    void recalculate_picked_gifts(pair<Price, vector<Gift>> &picked_gifts,
                                  const vector<pair<Price, Gift>> &sorted_prices) noexcept {
        for (Gift &i: picked_gifts.second) {
            i = sorted_prices[i].second;
        }
    }
private:
    // contains all employees and their underlings, can be viewed as a tree
    vector<Employee_relations> employees;
    // employee indexes sorted in a topsort order
    vector<Employee> employees_topsorted;
    // employee * gift table, used for calculating pre-sums of the optimal gifts for each employee
    vector<vector<Employee>> dynamic_table;
    // contains the cheapest possible gift for each employee (the lowest sum of all subordinate prices)
    vector<GiftPrice> cheapest_employee_gifts;
    // contains the second-cheapest possible gift for each employee
    vector<GiftPrice> second_cheapest_employee_gifts;

    vector<Employee> highest_bosses;
};


pair<Price, vector<Gift>> optimize_gifts(const vector<Employee> &boss, const vector<Price> &gift_prices) noexcept {

    Employee_database employee_database(boss);

    employee_database.topsort_employees();

    vector<pair<Price, Gift>> sorted_prices;
    for (size_t i=0; i < gift_prices.size(); i++) {
        sorted_prices.emplace_back(gift_prices[i], i);
    }
    sort(sorted_prices.begin(), sorted_prices.end());

    employee_database.populate_gifts_table(sorted_prices);

    pair<Price, vector<Gift>> picked_gifts;
    employee_database.pick_gifts(picked_gifts, sorted_prices);

    employee_database.recalculate_picked_gifts(picked_gifts, sorted_prices);

    return picked_gifts;
}


const std::tuple<Price, std::vector<Employee>, std::vector<Price>> EXAMPLES[] = {
        { 17, {1,2,3,4,NO_EMPLOYEE}, {25,4,18,3} },
        { 16, {4,4,4,4,NO_EMPLOYEE}, {25,4,18,3} },
        { 17, {4,4,3,4,NO_EMPLOYEE}, {25,4,18,3} },
        { 24, {4,4,3,4,NO_EMPLOYEE,3,3}, {25,4,18,3} },
        { 18, {1,4,5,1,NO_EMPLOYEE,4,2,NO_EMPLOYEE,NO_EMPLOYEE}, {4, 8, 23, 99, 1, 8}},
        { 23, {1,4,5,1,NO_EMPLOYEE,4,2,1,4,4}, {4, 8, 23, 99, 1, 8}},
        { 27, {NO_EMPLOYEE,0,1,2,0,0,0,0,0,0,0,1,1,1,1,1,2,2,2,3,3,3,3}, {4, 8, 23, 99, 1, 8, 2, 2, 3,5,3,7,8}}
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
