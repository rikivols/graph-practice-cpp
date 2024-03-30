# Array as AVL tree

This problem is to implement an array with AVL tree. This data structure can insert, delete, get ith element at O(log n) time complexity.

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description
Your task is to implement the **Array** class template parameterized by the type of elements T it stores. This class should have an interface like an array but should maintain elements in the structure of an AVL tree, thus achieving fast insertion and deletion at any position at the cost of a slight slowdown in access. The required methods are:

* **size()**: Returns the number of elements in the array.
* **operator\[](i)**: Returns the element at position i. Causes a std::out_of_range exception for positions outside the range [0, size()).
* **insert(i, t)**: Inserts element t at position i. Increases the index by 1 for elements at position i and higher. Causes a std::out_of_range exception for positions outside the range \[0, size()].
* **erase(i)**: Deletes the element at position i. Causes a std::out_of_range exception for positions outside the range [0, size()).

Copying or moving Array type objects is not tested. For testing purposes, the following static methods of the Array::TesterInterface class need to be implemented:

* **root(array)**: Returns a pointer to the root of the tree. (To the node, not to the value stored in it.)
* **parent(n)**: Returns a pointer to the parent of node n. Returns nullptr for the root. If your implementation does not have parent pointers and you have config::PARENT_POINTERS set to false, you can delete this method or always return nullptr.
* **right(n)**: Returns a pointer to the right child of n or nullptr if it does not exist.
* **left(n)**: Returns a pointer to the left child of n or nullptr if it does not exist.
* **value(n)**: Returns a const reference to the value stored in node n.

For easier development, the following configuration options are available in the template:

* **config::CHECK_DEPTH**: If true, it enables checking the shape of the AVL tree; otherwise, it only checks that the tree is a correct binary search. EVALUATION ignores this option. Default is false, turn it on only after you implement balancing.
* **config::PARENT_POINTERS**: Determines whether parent pointers should be tested. EVALUATION respects this option. Default is true.

Note: Testing the tree structure is recursive, so certain tests may end in a crash due to stack overflow for unbalanced trees.

There is a 5-second time limit for tests with a memory debugger, and 6 seconds for others.

Hint: It is recommended to first complete a mini-EVALUATION of the AVL tree and base your implementation on it.