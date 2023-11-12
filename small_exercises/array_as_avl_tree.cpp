#ifndef __PROGTEST__
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

// We use std::vector as a reference to check our implementation.
// It is not available in progtest :)
#include <vector>

template < typename T >
struct Ref {
    bool empty() const { return _data.empty(); }
    size_t size() const { return _data.size(); }

    const T& operator [] (size_t index) const { return _data.at(index); }
    T& operator [] (size_t index) { return _data.at(index); }

    void insert(size_t index, T value) {
        if (index > _data.size()) throw std::out_of_range("oops");
        _data.insert(_data.begin() + index, std::move(value));
    }

    T erase(size_t index) {
        T ret = std::move(_data.at(index));
        _data.erase(_data.begin() + index);
        return ret;
    }

    auto begin() const { return _data.begin(); }
    auto end() const { return _data.end(); }

private:
    std::vector<T> _data;
};

#endif


namespace config {
    inline constexpr bool PARENT_POINTERS = true;
    inline constexpr bool CHECK_DEPTH = true;
}

using namespace std;

template <typename T>
struct Node {
    Node *parent = nullptr;
    Node *right = nullptr;
    Node *left = nullptr;
    size_t key;
    T value;
    size_t height;
    size_t preSum;
};

template < typename T >
struct Tree {

    Node<T> *root = nullptr;
    size_t treeSize = 0;

    void recDeleteTree(Node<T> *currentNode) {
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

    int getHeight(Node<T> *currentNode) const {

        if (currentNode == nullptr) {
            return 0;
        }
        return currentNode->height;
    }

    int getPreSum(Node<T> *currentNode) const {
        if (currentNode == nullptr)
            return 0;
        return currentNode->preSum;
    }

    void updateNodeHeight(Node<T> *currentNode) {
        // updates height of a current node based on its kids
        currentNode->height = max(getHeight(currentNode->left), getHeight(currentNode->right)) + 1;
    }

    void updateNodeSum(Node<T> *currentNode) {
        currentNode->preSum = 1 + getPreSum(currentNode->left) + getPreSum(currentNode->right);
    }

    Node<T> *rotateToRight(Node<T> *currentNode) {
        Node<T> *y = currentNode->left;
        Node<T> *B = y->right;

        y->right = currentNode;
        y->parent = currentNode->parent;

        currentNode->left = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updateNodeHeight(currentNode);
        updateNodeHeight(y);

        updateNodeSum(currentNode);
        updateNodeSum(y);

        return y;
    }

    Node<T> *rotateToLeft(Node<T> *currentNode) {
        Node<T> *y = currentNode->right;
        Node<T> *B = y->left;

        y->left = currentNode;
        y->parent = currentNode->parent;

        currentNode->right = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updateNodeHeight(currentNode);
        updateNodeHeight(y);

        updateNodeSum(currentNode);
        updateNodeSum(y);

        return y;
    }

    int getDifference(Node<T> *currentNode) {
        if (currentNode == nullptr) {
            return 0;
        }
        return getHeight(currentNode->right) - getHeight(currentNode->left);
    }

    Node<T> * addNode(size_t key, const T &value) {
        Node<T> *node = new Node<T>();
        node->key = key;
        node->value = value;
        node->height = 1;
        node->preSum = 1;

        return node;
    }

    Node<T> *findNode(Node<T> *currentNode, size_t searchedKey, size_t pasPreSum=0) const {

        size_t currentIndex = getPreSum(currentNode->left) + pasPreSum;
        // we found our node's index
        if (currentIndex == searchedKey) {
            return currentNode;
        }
        // the found index is in the left part of the tree
        if (searchedKey < currentIndex) {
            return findNode(currentNode->left, searchedKey, pasPreSum);
        }
        return findNode(currentNode->right, searchedKey, currentIndex + 1);
    }

    T *find(size_t key) const {
        if (key > treeSize) {
            throw out_of_range("");
        }
        auto foundNode = findNode(root, key);
        return &(foundNode->value);
    };

    Node<T> * rebalanceInsert(Node<T> *currentNode) {

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

    Node<T> * insertNode(Node<T> *currentNode, Node<T> *previousNode, size_t key, T &value, bool &insertSuccess, long pastPreSum=0) {
        if (currentNode == nullptr) {
            Node<T> *node = addNode(key, value);
            node->parent = previousNode;
            return node;
        }

        size_t currentIndex = getPreSum(currentNode->left) + pastPreSum;

        // insert duplicates to the left (signals that it's newer)
        if (key <= currentIndex) {
            currentNode->left = insertNode(currentNode->left, currentNode, key, value, insertSuccess, pastPreSum);
        }
        if (key > currentIndex) {
            currentNode->right = insertNode(currentNode->right, currentNode, key, value, insertSuccess, currentIndex + 1);
        }

        updateNodeHeight(currentNode);
        updateNodeSum(currentNode);

        return rebalanceInsert(currentNode);
    }

    Node<T> *getSmallestNode(Node<T> *currentNode) {
        if (currentNode->left == nullptr) {
            return currentNode;
        }
        return getSmallestNode(currentNode->left);
    }

    bool insert(size_t key, T value) {
        bool insertSuccess = true;
        root = insertNode(root, nullptr, key, value, insertSuccess);
        if (insertSuccess) {
            treeSize++;
        }

        return insertSuccess;
    };

    Node<T> *eraseRebalance(Node<T> *currentNode) {
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

    Node<T> *eraseNode(Node<T> *currentNode, Node<T> *previousNode, size_t keyToDelete, bool & deleteSuccess, T &deletedValue,
                       long pastPreSum=0, bool alreadyUpdatedDeletedValue=false) {
        // not found
        if (currentNode == nullptr) {
            deleteSuccess = false;
            return nullptr;
        }

        size_t currentIndex = getPreSum(currentNode->left) + pastPreSum;

        // the value to delete is on the left side of the tree
        if (keyToDelete < currentIndex) {
            currentNode->left = eraseNode(currentNode->left, currentNode, keyToDelete, deleteSuccess, deletedValue, pastPreSum, alreadyUpdatedDeletedValue);
        }
            // the value to delete is on the right side of the tree
        else if (keyToDelete > currentIndex) {
            currentNode->right = eraseNode(currentNode->right, currentNode, keyToDelete, deleteSuccess, deletedValue, currentIndex + 1, alreadyUpdatedDeletedValue);
        }
            // we arrived at the node to delete
        else {
            // it was a leaf
            if (currentNode->left == nullptr and currentNode->right == nullptr) {
                if (!alreadyUpdatedDeletedValue) {
                    deletedValue = currentNode->value;
                }
                delete currentNode;
                return nullptr;
            }
            // node only had right child
            if (currentNode->left == nullptr) {
                Node<T> *rightNode = currentNode->right;
                rightNode->parent = previousNode;
                if (!alreadyUpdatedDeletedValue) {
                    deletedValue = currentNode->value;
                }
                delete currentNode;
                return rightNode;
            }
            // node only has left child
            if (currentNode->right == nullptr) {
                Node<T> *leftNode = currentNode->left;
                leftNode->parent = previousNode;
                if (!alreadyUpdatedDeletedValue) {
                    deletedValue = currentNode->value;
                }
                delete currentNode;
                return leftNode;
            }

            // get a node that's smallest from the right side
            Node<T> *minNode = getSmallestNode(currentNode->right);
            // replace the node's value, eliminating the value that we needed to remove
            deletedValue = currentNode->value;
            currentNode->key = minNode->key;
            currentNode->value = minNode->value;
            // erase the min node, since currently we have duplicates
            // delete index 0 from right tree
            currentNode->right = eraseNode(currentNode->right, currentNode, 0, deleteSuccess, deletedValue, 0, true);
        }

        updateNodeHeight(currentNode);
        updateNodeSum(currentNode);

        // root of the new tree
        return eraseRebalance(currentNode);
    }

    bool erase(size_t key, T& deletedValue) {
        bool deleteSuccess = true;
        Node<T> *newRoot = eraseNode(root, nullptr, key, deleteSuccess, deletedValue);
        if (!deleteSuccess) {
            return false;
        }
        root = newRoot;
        treeSize--;
        return true;
    };
};


// TODO implement
template < typename T >
struct Array {
    Tree<T> tree;

    bool empty() const {
        return tree.size() == 0;
    };

    size_t size() const {
        return tree.size();
    };

    const T& operator [] (size_t index) const {
        if (index >= size()) {
            throw out_of_range("");
        }
        return *tree.find(index);
    };

    T& operator [] (size_t index) {
        if (index >= size()) {
            throw out_of_range("");
        }
        return *tree.find(index);
    };

    void insert(size_t index, T value) {
        if (index > size()) {
            throw out_of_range("");
        }
        tree.insert(index, value);
    };

    T erase(size_t index) {
        if (index >= size()) {
            throw out_of_range("");
        }
        T deletedValue;
        tree.erase(index, deletedValue);
        return deletedValue;
    };

    // Needed to test the structure of the tree.
    // Replace Node with the real type of your nodes
    // and implementations with the ones matching
    // your attributes.
    struct TesterInterface {
        // using Node = ...
        static const Node<T> *root(const Array *t) { return t->tree.root; }
        // Parent of root must be nullptr, ignore if config::PARENT_POINTERS == false
        static const Node<T> *parent(const Node<T> *n) { return n->parent; }
        static const Node<T> *right(const Node<T> *n) { return n->right; }
        static const Node<T> *left(const Node<T> *n) { return n->left; }
        static const T& value(const Node<T> *n) { return n->value; }
    };
};


#ifndef __PROGTEST__

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

    size_t size() const {
        bool te = tested.empty();
        size_t r = ref.size();
        size_t t = tested.size();
        if (te != !t) throw TestFailed(fmt("Size: size %zu but empty is %s.",
                                           t, te ? "true" : "false"));
        if (r != t) throw TestFailed(fmt("Size: got %zu but expected %zu.", t, r));
        return r;
    }

    const T& operator [] (size_t index) const {
        const T& r = ref[index];
        const T& t = tested[index];
        if (r != t) throw TestFailed("Op [] const mismatch.");
        return t;
    }

    void assign(size_t index, T x) {
        ref[index] = x;
        tested[index] = std::move(x);
        operator[](index);
    }

    void insert(size_t i, T x, bool check_tree_ = false) {
        ref.insert(i, x);
        tested.insert(i, std::move(x));
//        cout << "I: " << i << " ";
//        tested.tree.printTree(tested.tree.root);
//        cout << endl;
//        tested.tree.print2D(tested.tree.root);
//        cout << endl;
        size();
        if (check_tree_) check_tree();
    }

    T erase(size_t i, bool check_tree_ = false) {
        T r = ref.erase(i);
        T t = tested.erase(i);

//        cout << "ref: " << r << ", my: " << t << endl;
//        cout << "TREE AFTER DELETE: " << endl;
//        tested.tree.printTree(tested.tree.root);
//        cout << endl;
//        tested.tree.print2D(tested.tree.root);
//        cout << endl;
        if (r != t) throw TestFailed(fmt("Erase mismatch at %zu.", i));
        size();
        if (check_tree_) check_tree();
        return t;
    }

    void check_tree() const {
        using TI = typename Array<T>::TesterInterface;
        auto ref_it = ref.begin();
        bool check_value_failed = false;
        auto check_value = [&](const T& v) {
            if (check_value_failed) return;
            check_value_failed = (ref_it == ref.end() || *ref_it != v);
            if (!check_value_failed) ++ref_it;
        };

        size();

        check_node(TI::root(&tested), decltype(TI::root(&tested))(nullptr), check_value);

        if (check_value_failed) throw TestFailed(
                    "Check tree: element mismatch");
    }

    template < typename Node, typename F >
    int check_node(const Node* n, const Node* p, F& check_value) const {
        if (!n) return -1;

        using TI = typename Array<T>::TesterInterface;
        if constexpr(config::PARENT_POINTERS) {
            if (TI::parent(n) != p) throw TestFailed("Parent mismatch.");
        }

        auto l_depth = check_node(TI::left(n), n, check_value);
        check_value(TI::value(n));
        auto r_depth = check_node(TI::right(n), n, check_value);

        if (config::CHECK_DEPTH && abs(l_depth - r_depth) > 1) throw TestFailed(fmt(
                    "Tree is not avl balanced: left depth %i and right depth %i.",
                    l_depth, r_depth
            ));

        return std::max(l_depth, r_depth) + 1;
    }

    static void _throw(const char *msg, bool s) {
        throw TestFailed(fmt("%s: ref %s.", msg, s ? "succeeded" : "failed"));
    }

    Array<T> tested;
    Ref<T> ref;
};


void test_insert() {
    Tester<int> t;

    for (int i = 0; i < 10; i++) {
        t.insert(i, i, true);
//        cout << "I: " << i << " ";
//        t.tested.tree.printTree(t.tested.tree.root);
//        cout << endl;
    };
    for (int i = 0; i < 10; i++) {
        t.insert(i, -i, true);

    }
    for (size_t i = 0; i < t.size(); i++) {
//        cout << "i: [" << i << "], my response: " << t.tested[i] << endl;
        t[i];
    }

    for (int i = 0; i < 5; i++) t.insert(15, (1 + i * 7) % 17, true);
    for (int i = 0; i < 10; i++) t.assign(2*i, 3*t[2*i]);
    for (size_t i = 0; i < t.size(); i++) t[i];
}

void test_erase() {
    Tester<int> t;

    for (int i = 0; i < 10; i++) t.insert(i, i, true);
    for (int i = 0; i < 10; i++) t.insert(i, -i, true);

    for (size_t i = 3; i < t.size(); i += 2) {
//        cout << "I: " << i << " ";
//        t.tested.tree.printTree(t.tested.tree.root);
//        cout << endl;
//        t.tested.tree.print2D(t.tested.tree.root);
//        cout << endl;
        t.erase(i, true);
    }
    for (size_t i = 0; i < t.size(); i++) t[i];

    for (int i = 0; i < 5; i++) t.insert(3, (1 + i * 7) % 17, true);
    for (size_t i = 1; i < t.size(); i += 3) t.erase(i, true);

    for (int i = 0; i < 20; i++) t.insert(3, 100 + i, true);

    for (int i = 0; i < 5; i++) t.erase(t.size() - 1, true);
    for (int i = 0; i < 5; i++) t.erase(0, true);

    for (int i = 0; i < 4; i++) t.insert(i, i, true);
    for (size_t i = 0; i < t.size(); i++) t[i];
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
        size_t pos = seq ? 0 : my_rand() % (i + 1);
        t.insert(pos, my_rand() % (3*size), check_tree);
    }

    t.check_tree();

    for (size_t i = 0; i < t.size(); i++) t[i];

    for (size_t i = 0; i < 30*size; i++) switch (my_rand() % 7) {
            case 1: {
                if (!erase && i % 3 == 0) break;
                size_t pos = seq ? 0 : my_rand() % (t.size() + 1);
                t.insert(pos, my_rand() % 1'000'000, check_tree);
                break;
            }
            case 2:
                if (erase) t.erase(my_rand() % t.size(), check_tree);
                break;
            case 3:
                t.assign(my_rand() % t.size(), 155 + i);
                break;
            default:
                t[my_rand() % t.size()];
        }

    t.check_tree();
}

int main() {
    try {
        std::cout << "Insert test..." << std::endl;
        test_insert();

        std::cout << "Erase test..." << std::endl;
        test_erase();
//
        std::cout << "Tiny random test..." << std::endl;
        test_random(20, CHECK_TREE);

        std::cout << "Small random test..." << std::endl;
        test_random(200, CHECK_TREE);

        std::cout << "Bigger random test..." << std::endl;
        test_random(5'000);

        std::cout << "Bigger sequential test..." << std::endl;
        test_random(5'000, SEQ);

        std::cout << "All tests passed." << std::endl;
    } catch (const TestFailed& e) {
        std::cout << "Test failed: " << e.what() << std::endl;
    }
}

#endif
