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
#include <stack>
#include <queue>
#include <random>
#include <type_traits>

#endif

using namespace std;

struct IndexedTree {

    struct Node {
        Node *parent = nullptr;
        Node *right = nullptr;
        Node *left = nullptr;
        size_t key;
        char value;
        size_t height;
        size_t preSum;
        size_t numberOfNewLines;
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

    ~IndexedTree() {
        recDeleteTree(root);
    }

    size_t size() const {
        return treeSize;
    };

    size_t getHeight(Node *currentNode) const {

        if (currentNode == nullptr) {
            return 0;
        }
        return currentNode->height;
    }

    size_t getPreSum(Node *currentNode) const {
        if (currentNode == nullptr)
            return 0;
        return currentNode->preSum;
    }

    size_t getNewLinesNum(Node *currentNode) const {
        if (currentNode == nullptr) {
            return 0;
        }
        return currentNode->numberOfNewLines;
    }

    void updateNodeHeight(Node *currentNode) {
        // updates height of a current node based on its kids
        currentNode->height = max(getHeight(currentNode->left), getHeight(currentNode->right)) + 1;
    }

    void updateNodeSum(Node *currentNode) {
        currentNode->preSum = 1 + getPreSum(currentNode->left) + getPreSum(currentNode->right);
    }

    void updateNumberOfNewLines(Node *currentNode) {
        currentNode->numberOfNewLines = getNewLinesNum(currentNode->left) + getNewLinesNum(currentNode->right);
        if (currentNode->value == '\n') {
            currentNode->numberOfNewLines++;
        }
    }

    void updatePrecalculatedValuesOfNode(Node *currentNode) {
        updateNodeHeight(currentNode);
        updateNodeSum(currentNode);
        updateNumberOfNewLines(currentNode);
    }

    Node *rotateToRight(Node *currentNode) {
        Node *y = currentNode->left;
        Node *B = y->right;

        y->right = currentNode;
        y->parent = currentNode->parent;

        currentNode->left = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updatePrecalculatedValuesOfNode(currentNode);
        updatePrecalculatedValuesOfNode(y);

        return y;
    }

    Node *rotateToLeft(Node *currentNode) {
        Node *y = currentNode->right;
        Node *B = y->left;

        y->left = currentNode;
        y->parent = currentNode->parent;

        currentNode->right = B;
        currentNode->parent = y;

        if (B != nullptr) {
            B->parent = currentNode;
        }

        updatePrecalculatedValuesOfNode(currentNode);
        updatePrecalculatedValuesOfNode(y);

        return y;
    }

    int getDifference(Node *currentNode) {
        if (currentNode == nullptr) {
            return 0;
        }
        return getHeight(currentNode->right) - getHeight(currentNode->left);
    }

    Node * addNode(size_t key, const char value) {
        Node *node = new Node();
        node->key = key;
        node->value = value;
        node->height = 1;
        node->preSum = 1;
        node->numberOfNewLines = 0;
        if (value == '\n') {
            node->numberOfNewLines = 1;
        }

        return node;
    }

    Node *findNode(Node *currentNode, size_t searchedKey, size_t &characterLine, size_t pasPreSum=0,
                   size_t pastNewLineNum=0) const {

        if (currentNode == nullptr) {
            return nullptr;
        }

        size_t currentNewLineNum = getNewLinesNum(currentNode->left) + pastNewLineNum;
        size_t currentIndex = getPreSum(currentNode->left) + pasPreSum;

        // we found our node's index
        if (currentIndex == searchedKey) {
            characterLine = currentNewLineNum;
            return currentNode;
        }

        if (currentNode->value == '\n') {
            currentNewLineNum++;
        }

        // the found index is in the left part of the tree
        if (searchedKey < currentIndex) {
            return findNode(currentNode->left, searchedKey, characterLine, pasPreSum, pastNewLineNum);
        }
        return findNode(currentNode->right, searchedKey, characterLine, currentIndex + 1, currentNewLineNum);
    }

    char *find(size_t key) const {
        if (key > treeSize) {
            throw out_of_range("");
        }
        size_t characterLine = 0;
        auto foundNode = findNode(root, key, characterLine);
        return &(foundNode->value);
    };

    size_t getNumberOfNewLinesFromIndex(size_t key) const {
        if (key >= treeSize) {
            throw out_of_range("");
        }
        size_t characterLine = 0;
        findNode(root, key, characterLine);
        return characterLine;
    };

    void printTree(Node *currentNode) {
        if(currentNode != nullptr) {
            printTree(currentNode->left);
            std::cout << "newlines: " << currentNode->numberOfNewLines << " key: " << currentNode->key << " value: " << currentNode->value << ", ";
            printTree(currentNode->right);
        }
    }

    Node *findEndOfLineRec(Node *currentNode, size_t endOfLineNumToGet, size_t &finalIndex,
                           size_t pastNewLineNum=0, size_t pastIndexPreSum=0) const {
        // finds end of the given line. Lines are indexed from 0

        if (currentNode == nullptr) {
            return nullptr;
        }

        size_t currentNewLineNum = getNewLinesNum(currentNode->left) + pastNewLineNum;
        size_t currentIndex = getPreSum(currentNode->left) + pastIndexPreSum;

        if (currentNode->value == '\n') {
            currentNewLineNum++;
        }
        // we found our node's index
        if (endOfLineNumToGet + 1 == currentNewLineNum and currentNode->value == '\n') {
            // this is index pointing at the last character of the line, add 1 to get the beginning of next line
            finalIndex = currentIndex;
            return currentNode;
        }
        // the found index is in the left part of the tree
        if (endOfLineNumToGet + 1 <= currentNewLineNum) {
            return findEndOfLineRec(currentNode->left, endOfLineNumToGet, finalIndex, pastNewLineNum, pastIndexPreSum);
        }

        return findEndOfLineRec(currentNode->right, endOfLineNumToGet, finalIndex, currentNewLineNum, currentIndex + 1);
    }

    size_t findStartOfLine(size_t lineNum) const {
        // gets first character of a given line, lines are indexed from 0

        size_t endOfLineIndex = 0;

        findEndOfLineRec(root, lineNum - 1, endOfLineIndex);

        // we found the end of previous line, we need to add 1 to get the start of next one
        return endOfLineIndex + 1;
    }

    size_t findEndOfLine(size_t lineNum) const {
        size_t endOfLineIndex = 0;

        Node *endNode = findEndOfLineRec(root, lineNum, endOfLineIndex);

        // end of line is not \n, it's the last line
        if (endNode == nullptr) {
            return size() - 1;
        }
        return endOfLineIndex;
    }

    Node * rebalanceInsert(Node *currentNode) {

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

    Node * insertNode(Node *currentNode, Node *previousNode, size_t key, char value, bool &insertSuccess, long pastPreSum=0) {
        if (currentNode == nullptr) {
            Node *node = addNode(key, value);
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

        updatePrecalculatedValuesOfNode(currentNode);

        return rebalanceInsert(currentNode);
    }

    Node *getSmallestNode(Node *currentNode) {
        if (currentNode->left == nullptr) {
            return currentNode;
        }
        return getSmallestNode(currentNode->left);
    }

    bool insert(size_t key, char value) {
        bool insertSuccess = true;
        root = insertNode(root, nullptr, key, value, insertSuccess);
        if (insertSuccess) {
            treeSize++;
        }

        return insertSuccess;
    };

    Node *eraseRebalance(Node *currentNode) {
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

    Node *eraseNode(Node *currentNode, Node *previousNode, size_t keyToDelete, bool & deleteSuccess, char &deletedValue,
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
                Node *rightNode = currentNode->right;
                rightNode->parent = previousNode;
                if (!alreadyUpdatedDeletedValue) {
                    deletedValue = currentNode->value;
                }
                delete currentNode;
                return rightNode;
            }
            // node only has left child
            if (currentNode->right == nullptr) {
                Node *leftNode = currentNode->left;
                leftNode->parent = previousNode;
                if (!alreadyUpdatedDeletedValue) {
                    deletedValue = currentNode->value;
                }
                delete currentNode;
                return leftNode;
            }

            // get a node that's smallest from the right side
            Node *minNode = getSmallestNode(currentNode->right);
            // replace the node's value, eliminating the value that we needed to remove
            deletedValue = currentNode->value;
            currentNode->key = minNode->key;
            currentNode->value = minNode->value;
            // erase the min node, since currently we have duplicates
            // delete index 0 from right tree
            currentNode->right = eraseNode(currentNode->right, currentNode, 0, deleteSuccess, deletedValue, 0, true);
        }

        updatePrecalculatedValuesOfNode(currentNode);

        // root of the new tree
        return eraseRebalance(currentNode);
    }

    bool erase(size_t key, char &deletedValue) {
        bool deleteSuccess = true;
        Node *newRoot = eraseNode(root, nullptr, key, deleteSuccess, deletedValue);
        if (!deleteSuccess) {
            return false;
        }
        root = newRoot;
        treeSize--;
        return true;
    };
};


struct ArrayAvlTree {
    IndexedTree tree;

    bool empty() const {
        return tree.size() == 0;
    };

    size_t size() const {
        return tree.size();
    };

    const char operator [] (size_t index) const {
        if (index >= size()) {
            throw out_of_range("");
        }
        return *tree.find(index);
    };

    char& operator [] (size_t index) {
        if (index >= size()) {
            throw out_of_range("");
        }
        return *tree.find(index);
    };

    void insert(size_t index, char value) {
        if (index > size()) {
            throw out_of_range("");
        }
        tree.insert(index, value);
    };

    char erase(size_t index) {
        if (index >= size()) {
            throw out_of_range("");
        }
        char deletedValue;
        tree.erase(index, deletedValue);
        return deletedValue;
    };
};


struct TextEditorBackend {
    // stores whole text
    ArrayAvlTree textAvlArray;

    TextEditorBackend(const std::string& text) {
        // stores new lines
        for (auto v: text) {
            textAvlArray.insert(textAvlArray.size(), v);
        }
    };

    size_t size() const {
        return textAvlArray.size();
    };

    size_t lines() const {
        if (textAvlArray.empty()) {
            return 1;
        }
        return textAvlArray.tree.getNewLinesNum(textAvlArray.tree.root) + 1;
    };

    char at(size_t i) const {
        return textAvlArray[i];
    };

    void edit(size_t i, char c) {


        textAvlArray.erase(i);
        textAvlArray.insert(i, c);
    };

    void insert(size_t i, char c) {
        textAvlArray.insert(i, c);
    };

    void erase(size_t i) {
        textAvlArray.erase(i);
    };

    size_t line_start(size_t r) const {
        if (r >= lines()) {
            throw out_of_range("");
        }
        if (r == 0) {
            return 0;
        }
        return textAvlArray.tree.findStartOfLine(r);
    };
    size_t line_length(size_t r) const {
        if (lines() == 0) {
            return textAvlArray.size();
        }

        size_t startOfLineIndex = line_start(r);

        size_t endOfLineIndex = textAvlArray.tree.findEndOfLine(r);
        return endOfLineIndex - startOfLineIndex + 1;
    };
    size_t char_to_line(size_t i) const {
        return textAvlArray.tree.getNumberOfNewLinesFromIndex(i);
    };
};


#ifndef __PROGTEST__

////////////////// Dark magic, ignore ////////////////////////

template < typename T >
auto quote(const T& t) { return t; }

std::string quote(const std::string& s) {
    std::string ret = "\"";
    for (char c : s) if (c != '\n') ret += c; else ret += "\\n";
    return ret + "\"";
}

#define STR_(a) #a
#define STR(a) STR_(a)

#define CHECK_(a, b, a_str, b_str) do { \
    auto _a = (a); \
    decltype(a) _b = (b); \
    if (_a != _b) { \
      std::cout << "Line " << __LINE__ << ": Assertion " \
        << a_str << " == " << b_str << " failed!" \
        << " (lhs: " << quote(_a) << ")" << std::endl; \
      fail++; \
    } else ok++; \
  } while (0)

#define CHECK(a, b) CHECK_(a, b, #a, #b)

#define CHECK_ALL(expr, ...) do { \
    std::array _arr = { __VA_ARGS__ }; \
    for (size_t _i = 0; _i < _arr.size(); _i++) \
      CHECK_((expr)(_i), _arr[_i], STR(expr) "(" << _i << ")", _arr[_i]); \
  } while (0)

#define CHECK_EX(expr, ex) do { \
    try { \
      (expr); \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but no exception was raised." << std::endl; \
    } catch (const ex&) { ok++; \
    } catch (...) { \
      fail++; \
      std::cout << "Line " << __LINE__ << ": Expected " STR(expr) \
        " to throw " #ex " but got different exception." << std::endl; \
    } \
  } while (0)

////////////////// End of dark magic ////////////////////////


std::string text(const TextEditorBackend& t) {
    std::string ret;
    for (size_t i = 0; i < t.size(); i++) ret.push_back(t.at(i));
    return ret;
}

void test1(int& ok, int& fail) {
    TextEditorBackend s("123\n456\n789");
    CHECK(s.size(), 11);
    CHECK(text(s), "123\n456\n789");
    CHECK(s.lines(), 3);
    // 5th character of text is at 1st line
    CHECK_ALL(s.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2);
    // line_start(0) = 0, line_start(1) = 4, line_start(2) = 8
    CHECK_ALL(s.line_start, 0, 4, 8);
    CHECK_ALL(s.line_length, 4, 4, 3);
}

void test2(int& ok, int& fail) {
    TextEditorBackend t("123\n456\n789\n");
    CHECK(t.size(), 12);
    CHECK(text(t), "123\n456\n789\n");
    CHECK(t.lines(), 4);
    CHECK_ALL(t.char_to_line, 0,0,0,0, 1,1,1,1, 2,2,2,2);
    CHECK_ALL(t.line_start, 0, 4, 8, 12);
    CHECK_ALL(t.line_length, 4, 4, 4, 0);
}

void test3(int& ok, int& fail) {
    TextEditorBackend t("asdfasdfasdf");

    CHECK(t.size(), 12);
    CHECK(text(t), "asdfasdfasdf");
    CHECK(t.lines(), 1);
    CHECK_ALL(t.char_to_line, 0,0,0,0, 0,0,0,0, 0,0,0,0);
    CHECK(t.line_start(0), 0);
    CHECK(t.line_length(0), 12);

    t.insert(0, '\n');
    CHECK(t.size(), 13);
    CHECK(text(t), "\nasdfasdfasdf");
    CHECK(t.lines(), 2);
    CHECK_ALL(t.line_start, 0, 1);

    t.insert(4, '\n');
    CHECK(t.size(), 14);
    CHECK(text(t), "\nasd\nfasdfasdf");
    CHECK(t.lines(), 3);
    CHECK_ALL(t.line_start, 0, 1, 5);

    t.insert(t.size(), '\n');
    CHECK(t.size(), 15);
    CHECK(text(t), "\nasd\nfasdfasdf\n");
    CHECK(t.lines(), 4);
    CHECK_ALL(t.line_start, 0, 1, 5, 15);

    t.edit(t.size() - 1, 'H');
    CHECK(t.size(), 15);
    CHECK(text(t), "\nasd\nfasdfasdfH");
    t.textAvlArray.tree.printTree(t.textAvlArray.tree.root);
    cout << endl;
    CHECK(t.lines(), 3);
    CHECK_ALL(t.line_start, 0, 1, 5);

    t.erase(8);
    CHECK(t.size(), 14);
    CHECK(text(t), "\nasd\nfasfasdfH");
    CHECK(t.lines(), 3);
    CHECK_ALL(t.line_start, 0, 1, 5);

    t.erase(4);
    CHECK(t.size(), 13);
    CHECK(text(t), "\nasdfasfasdfH");
    CHECK(t.lines(), 2);
    CHECK_ALL(t.line_start, 0, 1);
}

void test_ex(int& ok, int& fail) {
    TextEditorBackend t("123\n456\n789\n");
    CHECK_EX(t.at(12), std::out_of_range);

    CHECK_EX(t.insert(13, 'a'), std::out_of_range);
    CHECK_EX(t.edit(12, 'x'), std::out_of_range);
    CHECK_EX(t.erase(12), std::out_of_range);

    CHECK_EX(t.line_start(4), std::out_of_range);
    CHECK_EX(t.line_start(40), std::out_of_range);
    CHECK_EX(t.line_length(4), std::out_of_range);
    CHECK_EX(t.line_length(6), std::out_of_range);
    CHECK_EX(t.char_to_line(12), std::out_of_range);
    CHECK_EX(t.char_to_line(25), std::out_of_range);
}

int main() {
    int ok = 0, fail = 0;
    if (!fail) test1(ok, fail);
    if (!fail) test2(ok, fail);
    if (!fail) test3(ok, fail);
    if (!fail) test_ex(ok, fail);

    if (!fail) std::cout << "Passed all " << ok << " tests!" << std::endl;
    else std::cout << "Failed " << fail << " of " << (ok + fail) << " tests." << std::endl;
}

#endif


