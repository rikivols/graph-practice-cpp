# AVL Tree

This problem is to implement AVL Tree

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description
Your task is to implement the **Tree** class template parameterized by the type of elements T it stores. This class should maintain elements in the structure of an AVL tree according to the std::less<T> ordering (or < operator). The required methods are:

* **size()**: Returns the number of elements in the tree.
* **find(t)**: Finds the element t in the tree and returns a const pointer to it, or nullptr if the element is not in the tree.
* **insert(t)**: Attempts to insert t. Returns whether the insertion was successful. Insertion fails if the tree already contains an element with the same value.
* **erase(t)**: Deletes the element t if it is in the tree. Returns whether the element was deleted.

Copying or moving Tree type objects is not tested. For testing purposes, the following static methods of the Tree::TesterInterface class need to be implemented:

* **root(tree)**: Returns a pointer to the root of the tree. (To the node, not to the value stored in it.)
* **parent(n)**: Returns a pointer to the parent of node n. Returns nullptr for the root. If your implementation does not have parent pointers and you have config::PARENT_POINTERS set to false, you can delete this method or always return nullptr.
* **right(n)**: Returns a pointer to the right child of n or nullptr if it does not exist.
* **left(n)**: Returns a pointer to the left child of n or nullptr if it does not exist.
* **value(n)**: Returns a const reference to the value stored in node n.

For easier development, the following configuration options are available in the template:

* **config::CHECK_DEPTH**: If true, it enables checking the shape of the AVL tree; otherwise, it only checks that the tree is a correct binary search. EVALUATION ignores this option. Default is false, turn it on only after you implement balancing.
* **config::PARENT_POINTERS**: Determines whether parent pointers should be tested. EVALUATION respects this option at the cost of a minor point loss if parent pointers are not implemented. Default is true.

Note: Testing the tree structure is recursive, so certain tests may end in a crash due to stack overflow for unbalanced trees.

There is a 5-second time limit for tests with a memory debugger, and 3 seconds for others.