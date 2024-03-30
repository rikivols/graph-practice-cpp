# Breadth First Search

This problem is to practice Breadth First Search.

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description
Your task is to implement the function size_t bfs(const Graph& G, Vertex u, std::vector<Vertex>& P, std::vector<size_t>& D), which performs a breadth-first search. The arguments are:

* **Graph G**: The graph to be searched. The description of the Graph class is below.
* **Vertex u**: The vertex from which the search begins.
* **std::vector<Vertex>& P**: The predecessor array. Before calling the bfs function, it is always initialized to the correct size and filled with the constant NO_VERTEX.
* **std::vector<size_t>& D**: The distance array. Before calling the bfs function, it is always initialized to the correct size and filled with the constant NO_DISTANCE.
The return value is the number of visited vertices, and the arrays P and D must be correctly populated. Set the predecessor of the starting vertex to the constant ROOT. Your implementation should be linear in the size of the visited part of the graph.

The Graph class represents an undirected or directed graph. Important methods are:

* **vertices()**: The number of vertices.
* **operator\[](Vertex v)**: The list of neighbors (or successors) of vertex v. Vertices are integers starting from 0.
* **Methods begin() and end()**, which allow iteration over vertices and the use of the Graph class in a range-for loop as follows: for (Vertex v : G) ....
Time limits are 5 seconds for a small test and 3.5 seconds for a large one.