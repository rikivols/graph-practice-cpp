# Topsort

This problem is to practice and implement topsort.

`skeletonToFill.cpp` contains tests and interface to fill your solution to.

## Description
Your task is to implement the function **std::pair<bool, std::vector<Vertex>> topsort(const Graph& G)**. The return value is either:

* **true** and a list of all vertices in topological order, or
* **false** and a list of vertices that form a cycle (an edge must also lead from the last vertex to the first in the list).

The Graph class represents a directed graph. Important methods are:

* **vertices()**: The number of vertices.
* **operator\[](Vertex v)**: The list of neighbors of vertex v. Vertices are integers starting from 0.
* **Methods begin() and end()**, which allow iteration over vertices and the use of the Graph class in a range-for loop as follows: for (Vertex v : G) ....
* **reversed()**: Returns a new graph created by reversing the orientation of all edges.

Time limits are 5 seconds for a small test and 3 seconds for a large one.