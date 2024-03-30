# Binary heap

This problem is to practice Binary Heap. It's separated into two parts, v1 and v2.

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description v1
Your task is to implement the class template **template <typename T, typename Comp> struct BinaryHeap**, which represents a binary heap with elements of type T sorted according to the comparator Comp stored in an array. As is customary in C++, comp(a, b) == true means that a is strictly less than b. The required methods are:

* A default constructor and a constructor with a parameter of type **Comp**. The default constructor should create a comparator using its default constructor.
* **empty()**: Determines whether the heap is empty.
* **size()**: Returns the number of elements in the heap.
* **min()**: Returns the current minimum. Throws a std::out_of_range exception if the heap is empty.
* **extract_min()**: Returns the minimum and simultaneously removes it from the heap. Throws a std::out_of_range exception if the heap is empty. Must return the same minimum as the min() method.
* **push(t)**: Inserts the element t into the heap. It is advisable to take the argument by value and move it using std::move.

Additionally, the BinaryHeap template must contain the TestHelper structure, which will allow testing the heap structure. TestHelper must contain the following static methods:

* **index_to_value(H, i)**: Returns a reference to the i-th element in the heap H.
* **root_index()**: The index of the root of the heap.
* **parent(i)**: The index of the parent of the element at position i.
* **child(p, i)**: The index of the i-th (i.e., zeroth or first) child of the node at position p.

You may assume that T has a default constructor and a move constructor and assignment operator. Conversely, T may not need to be copyable. BinaryHeap class objects are not moved or copied in tests.

Time limits are 8 seconds for a small test and 3.2 seconds for a large one.

## Description v2
This assignment is an extension of the Binary Heap I example with operations for changing the value and deleting an element. These operations are specific in that they require some form of reference to an element in the heap. This reference must remain valid when the element moves within the heap.

Since this is a rather technical problem not closely related to algorithmization, the implementation of such a reference has been prepared for you using the Node and Ref classes. Store Node in the array and pass a reference to the just inserted Node to the Ref constructor. Whether you use this or write your own is up to you.

Modifications to BinaryHeap compared to Binary Heap I are:

* **Type Ref**, which represents a reference to an element. This type must have a default constructor and be movable. Copying is not required. It must also implement unary operators * and ->, i.e., behave like a const pointer to the given value in the heap.
* **push(t)** must return a reference to the inserted element.
* **erase(ref)**: Deletes the element designated by the reference ref from the heap and returns its value.
* **change(ref, f)**: Calls the functor f (its type is a template parameter) with a non-const reference to the element designated by ref, thereby allowing its value to be changed. The functor f must be called exactly once. Upon completion of this call, it fixes the heap structure.

Time limits are 5 seconds for a small test and 2.1 seconds for a large one.

Free tip: It might be a good idea to implement a helper method ref_to_index, which converts an object of type Ref to the index of the given element in the heap.