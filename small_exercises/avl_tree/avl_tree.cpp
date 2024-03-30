#ifndef __EVALUATION__
#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <array>
#include <random>
#include <type_traits>

// We use std::set as a reference to check our implementation.
// It is not available in EVALUATION :)
#include <set>

template < typename T >
struct Ref {
    size_t size() const { return _data.size(); }
    const T* find(const T& value) const {
        auto it = _data.find(value);
        if (it == _data.end()) return nullptr;
        return &*it;
    }
    bool insert(const T& value) { return _data.insert(value).second; }
    bool erase(const T& value) { return _data.erase(value); }

    auto begin() const { return _data.begin(); }
    auto end() const { return _data.end(); }

private:
    std::set<T> _data;
};

#endif


namespace config {
    // Enable to check that the tree is AVL balanced.
    inline constexpr bool CHECK_DEPTH = true;

    // Disable if your implementation does not have parent pointers
    inline constexpr bool PARENT_POINTERS = true;
}


// TODO implement
using namespace std;

template < typename T >
struct Tree {
    struct Node {
        Node *parent = nullptr;
        Node *right = nullptr;
        Node *left = nullptr;
        T value;
        size_t height;
    };

    Node *root = nullptr;
    size_t treeSize = 0;

    void recDeleteTree(Node *currentNode) {
        if (currentNode == nullptr) {
            return;
        }
        recDeleteTree(currentNode->left);
        recDeleteTree(currentNode->right);

        delete currentNode;
    }

    ~Tree() {
        recDeleteTree(root);
    }

    size_t size() const {
        return treeSize;
    };

    int getHeight(Node *currentNode) {

        if (currentNode == nullptr) {
            return 0;
        }
        return currentNode->height;
    }

    void updateNodeHeight(Node * currentNode) {
        // updates height of a current node based on its kids
        currentNode->height = max(getHeight(currentNode->left), getHeight(currentNode->right)) + 1;
    }

    Node* rotateToRight(Node *currentNode) {
        Node *y = currentNode->left;
        Node *B = y->right;

        y->right = currentNode;
        y->parent = currentNode->parent;

        currentNode->left = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updateNodeHeight(currentNode);
        updateNodeHeight(y);

        return y;
    }

    Node* rotateToLeft(Node *currentNode) {
        Node *y = currentNode->right;
        Node *B = y->left;

        y->left = currentNode;
        y->parent = currentNode->parent;

        currentNode->right = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updateNodeHeight(currentNode);
        updateNodeHeight(y);

        return y;
    }

    int getDifference(Node *currentNode) {
        if (currentNode == nullptr) {
            return 0;
        }
        return getHeight(currentNode->right) - getHeight(currentNode->left);
    }

    Node * addNode(const T &value) {
        Node *node = new Node();
        node->value = value;
        node->height = 1;

        return node;
    }

    const Node * findNode(Node *currentNode, const T& searchedValue) const {
        if (currentNode == nullptr) {
            return nullptr;
        }
        if (currentNode->value == searchedValue) {
            return currentNode;
        }
        if (searchedValue < currentNode->value) {
            return findNode(currentNode->left, searchedValue);
        }
        return findNode(currentNode->right, searchedValue);
    }

    const T* find(const T &value) const {
        auto foundNode = findNode(root, value);
        if (foundNode == nullptr) {
            return nullptr;
        }
        return &(foundNode->value);
    };

    Node * rebalanceInsert(Node *currentNode, const T &valueToInsert) {

        int currentDifference = getDifference(currentNode);
        int leftNodeDifference = getDifference(currentNode->left);
        int rightNodeDifference = getDifference(currentNode->right);

        // left subtree is bigger by at least 2
        if (currentDifference < -1) {
            if (leftNodeDifference < 1) {
                return rotateToRight(currentNode);
            }

            // double rotation
            currentNode->left = rotateToLeft(currentNode->left);
            return rotateToRight(currentNode);
        }
        // right subtree is bigger by at least 2
        if (currentDifference > 1) {
            if (rightNodeDifference > -1) {
                return rotateToLeft(currentNode);
            }
            currentNode->right = rotateToRight(currentNode->right);
            return rotateToLeft(currentNode);
        }

        return currentNode;
    }

    Node * insertNode(Node *currentNode, Node *previousNode, T &valueToInsert, bool &insertSuccess) {
        if (currentNode == nullptr) {
            Node *node = addNode(valueToInsert);
            node->parent = previousNode;
            return node;
        }
        if (currentNode->value == valueToInsert) {
            insertSuccess = false;
            return currentNode;
        }

        if (valueToInsert < currentNode->value) {
            currentNode->left = insertNode(currentNode->left, currentNode, valueToInsert, insertSuccess);
        }
        if (valueToInsert > currentNode->value) {
            currentNode->right = insertNode(currentNode->right, currentNode, valueToInsert, insertSuccess);
        }

        updateNodeHeight(currentNode);

        return rebalanceInsert(currentNode, valueToInsert);
    }

    Node * getSmallestNode(Node *currentNode) {
        if (currentNode->left == nullptr) {
            return currentNode;
        }
        return getSmallestNode(currentNode->left);
    }

    bool insert(T value) {
        bool insertSuccess = true;
        root = insertNode(root, nullptr, value, insertSuccess);
        if (insertSuccess) {
            treeSize++;
        }

        return insertSuccess;
    };

    Node * eraseRebalance(Node *currentNode) {
        int currentDifference = getDifference(currentNode);
        int leftNodeDifference = getDifference(currentNode->left);
        int rightNodeDifference = getDifference(currentNode->right);

        if (currentDifference < -1) {
            if (leftNodeDifference < 1) {
                return rotateToRight(currentNode);
            }
            currentNode->left = rotateToLeft(currentNode->left);
            return rotateToRight(currentNode);
        }

        if (currentDifference > 1) {
            if (rightNodeDifference > -1) {
                return rotateToLeft(currentNode);
            }
            currentNode->right = rotateToRight(currentNode->right);
            return rotateToLeft(currentNode);
        }

        return currentNode;
    }

    Node * eraseNode(Node *currentNode, Node *previousNode, const T &valueToDelete, bool & deleteSuccess) {
        // not found
        if (currentNode == nullptr) {
            deleteSuccess = false;
            return nullptr;
        }

        // the value to delete is on the left side of the tree
        if (valueToDelete < currentNode->value) {
            currentNode->left = eraseNode(currentNode->left, currentNode, valueToDelete, deleteSuccess);
        }
        // the value to delete is on the right side of the tree
        else if (valueToDelete > currentNode->value) {
            currentNode->right = eraseNode(currentNode->right, currentNode, valueToDelete, deleteSuccess);
        }
        // we arrived at the node to delete
        else {
            // it was a leaf
            if (currentNode->left == nullptr and currentNode->right == nullptr) {
                delete currentNode;
                return nullptr;
            }
            // node only had right child
            if (currentNode->left == nullptr) {
                Node *rightNode = currentNode->right;
                rightNode->parent = previousNode;
                delete currentNode;
                return rightNode;
            }
            // node only has left child
            if (currentNode->right == nullptr) {
                Node *leftNode = currentNode->left;
                leftNode->parent = previousNode;
                delete currentNode;
                return leftNode;
            }

            // get a node that's smallest from the right side
            Node *minNode = getSmallestNode(currentNode->right);
            // replace the node's value, eliminating the value that we needed to remove
            currentNode->value = minNode->value;
            // erase the min node, since currently we have duplicates
            currentNode->right = eraseNode(currentNode->right, currentNode, minNode->value, deleteSuccess);
        }

        updateNodeHeight(currentNode);

        // root of the new tree
        return eraseRebalance(currentNode);
    }

    bool erase(const T& value) {
        bool deleteSuccess = true;
        Node *newRoot = eraseNode(root, nullptr, value, deleteSuccess);
        if (!deleteSuccess) {
            return false;
        }
        root = newRoot;
        treeSize--;
        return true;
    };

    // Needed to test the structure of the tree.
    // Replace Node with the real type of your nodes
    // and implementations with the ones matching
    // your attributes.

    struct TesterInterface {
        // using Node = ...
        static const Node *root(const Tree *t) { return t->root; }
        // Parent of root must be nullptr, ignore if config::PARENT_POINTERS == false
        static const Node *parent(const Node *n) { return n->parent; }
        static const Node *right(const Node *n) { return n->right; }
        static const Node *left(const Node *n) { return n->left; }
        static const T& value(const Node *n) { return n->value; }
    };
};


#ifndef __EVALUATION__

struct TestFailed : std::runtime_error {
    using std::runtime_error::runtime_error;
};

std::string fmt(const char *f, ...) {
    va_list args1;
    va_list args2;
    va_start(args1, f);
    va_copy(args2, args1);

    std::string buf(vsnprintf(nullptr, 0, f, args1), '\0');
    va_end(args1);

    vsnprintf(buf.data(), buf.size() + 1, f, args2);
    va_end(args2);

    return buf;
}


template < typename T >
struct Tester {
    Tester() = default;

    void size() const {
        size_t r = ref.size();
        size_t t = tested.size();
        if (r != t) throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
    }

    void find(const T& x) const {
        auto r = ref.find(x);
        auto t = tested.find(x);
        bool found_r = r != nullptr;
        bool found_t = t != nullptr;

        if (found_r != found_t) _throw("Find mismatch", found_r);
        if (found_r && *t != x) throw TestFailed("Find: found different value");
    }

    void insert(const T& x, bool check_tree_ = false) {
        auto succ_r = ref.insert(x);
        auto succ_t = tested.insert(x);
        if (succ_r != succ_t) _throw("Insert mismatch", succ_r);
        size();
        if (check_tree_) check_tree();
    }

    void erase(const T& x, bool check_tree_ = false) {
        bool succ_r = ref.erase(x);
        auto succ_t = tested.erase(x);
        if (succ_r != succ_t) _throw("Erase mismatch", succ_r);
        size();
        if (check_tree_) check_tree();
    }

    struct NodeCheckResult {
        const T* min = nullptr;
        const T* max = nullptr;
        int depth = -1;
        size_t size = 0;
    };

    void check_tree() const {
        using TI = typename Tree<T>::TesterInterface;
        auto ref_it = ref.begin();
        bool check_value_failed = false;
        auto check_value = [&](const T& v) {
            if (check_value_failed) return;
            check_value_failed = (ref_it == ref.end() || *ref_it != v);
            if (!check_value_failed) ++ref_it;
        };

        auto r = check_node(TI::root(&tested), decltype(TI::root(&tested))(nullptr), check_value);
        size_t t_size = tested.size();

        if (t_size != r.size) throw TestFailed(
                    fmt("Check tree: size() reports %zu but expected %zu.", t_size, r.size));

        if (check_value_failed) throw TestFailed(
                    "Check tree: element mismatch");
        size();
    }

    template < typename Node, typename F >
    NodeCheckResult check_node(const Node* n, const Node* p, F& check_value) const {
        if (!n) return {};

        using TI = typename Tree<T>::TesterInterface;
        if constexpr(config::PARENT_POINTERS) {
            if (TI::parent(n) != p) throw TestFailed("Parent mismatch.");
        }

        auto l = check_node(TI::left(n), n, check_value);
        check_value(TI::value(n));
        auto r = check_node(TI::right(n), n, check_value);

        if (l.max && !(*l.max < TI::value(n)))
            throw TestFailed("Max of left subtree is too big.");
        if (r.min && !(TI::value(n) < *r.min))
            throw TestFailed("Min of right subtree is too small.");

        if (config::CHECK_DEPTH && abs(l.depth - r.depth) > 1) throw TestFailed(fmt(
                    "Tree is not avl balanced: left depth %i and right depth %i.",
                    l.depth, r.depth
            ));

        return {
                l.min ? l.min : &TI::value(n),
                r.max ? r.max : &TI::value(n),
                std::max(l.depth, r.depth) + 1, 1 + l.size + r.size
        };
    }

    static void _throw(const char *msg, bool s) {
        throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
    }

    Tree<T> tested;
    Ref<T> ref;
};


void test_insert() {
    Tester<int> t;

    for (int i = 0; i < 10; i++) {
        t.insert(i, true);
    }
    for (int i = -10; i < 20; i++) {
        t.find(i);
    }

    for (int i = 0; i < 10; i++) {
        t.insert((1 + i * 7) % 17, true);
    }
    for (int i = -10; i < 20; i++) t.find(i);
}


void test_erase() {
    Tester<int> t;

    for (int i = 0; i < 10; i++) t.insert((1 + i * 7) % 17, true);
    for (int i = -10; i < 20; i++) t.find(i);

    for (int i = 3; i < 22; i += 2) {
        t.erase(i, true);
    }
    for (int i = -10; i < 20; i++) t.find(i);

    for (int i = 0; i < 10; i++) {
        t.insert((1 + i * 13) % 17 - 8, true);
    }

    for (int i = -10; i < 20; i++) t.find(i);

    for (int i = -4; i < 1; i++) {
        t.erase(i, true);
    };

    for (int i = 1; i < 10; i++) {
        t.erase(i, true);
    };
    for (int i = -10; i < 20; i++) {
        t.find(i);
    }
}

enum RandomTestFlags : unsigned {
    SEQ = 1, NO_ERASE = 2, CHECK_TREE = 4
};

void test_random(size_t size, unsigned flags = 0) {
    Tester<size_t> t;
    std::mt19937 my_rand(24707 + size);

    bool seq = flags & SEQ;
    bool erase = !(flags & NO_ERASE);
    bool check_tree = flags & CHECK_TREE;

    for (size_t i = 0; i < size; i++) {

        t.insert(seq ? 2 * i : my_rand() % (3 * size), check_tree);
    }
    t.check_tree();

    for (size_t i = 0; i < 3*size + 1; i++) t.find(i);


    for (size_t i = 0; i < 30*size; i++) switch (my_rand() % 5) {
            case 1: t.insert(my_rand() % (3*size), check_tree);
                break;
            case 2: if (erase) t.erase(my_rand() % (3*size), check_tree);
                break;
            default:
                t.find(my_rand() % (3*size));
        }

    t.check_tree();
}

int main() {
    try {
        std::cout << "Insert test..." << std::endl;
        test_insert();

        std::cout << "Erase test..." << std::endl;
        test_erase();

        std::cout << "Tiny random test..." << std::endl;
        test_random(20, CHECK_TREE);

        std::cout << "Small random test..." << std::endl;
        test_random(200, CHECK_TREE);

        std::cout << "Big random test..." << std::endl;
        test_random(50'000);

        std::cout << "Big sequential test..." << std::endl;
        test_random(50'000, SEQ);

        std::cout << "All tests passed." << std::endl;
    } catch (const TestFailed& e) {
        std::cout << "Test failed: " << e.what() << std::endl;
    }
}

#endif


