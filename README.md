# kdtree
kdtree module for C++

[![Build Status](https://travis-ci.org/questbeat/kdtree.png?branch=master)](https://travis-ci.org/questbeat/kdtree)


## Installation
1. Copy `kdtree.hpp` and `node.hpp` to your project.
2. Write `#include "kdtree.hpp"` in your code.


## Usage
### Namespace
This module uses namespace `kdtree`.  
Write `using namespace kdtree;` if necessary.

### Classes
* `kdtree` : A class having a pointer to a root node of the tree.
* `node` : A class representing a node of the tree.

### Create a node
Generally you don't have to create a node directly, but it might be good to know hot to create a node and access to the data.

`node` is a template class and you can use any classes having the member `x` and `y`.  

    node<cv::Point> *node = new node<cv::Point>(cv::Point(10, 0));

The member `point` is representing the point of the node.

    cout << node->point << endl; // [10, 0]

Read the header if you want to know about other methods and members.

### Create a tree
`kdtree` is a template class and you can use any classes having the member `x` and `y`.  
Following example uses `cv::Point` of OpenCV as a point of the tree.

    vector<cv::Point> points = {
        cv::Point(10, 0),
        cv::Point(20, 0),
        cv::Point(40, 0),
        cv::Point(80, 0),
        cv::Point(160, 0)
    };
    
    kdtree<cv::Point> *tree = new kdtree<cv::Point>(points);

### Delete a tree
Just `delete` the instance of the tree.

    delete tree;
    
### Nearest neighbor search
Use `nearest()`.

In this example the result is the node having `cv::Point(40, 0)`.

    node<cv::Point> *nearest_neighbor = tree->nearest(cv::Point(50, 0));

In this example the result is the node having `cv::Point(80, 0)`.

    node<cv::Point> *nearest_neighbor = tree->nearest(cv::Point(80, 0));

### Radius nearest neighbor search
Use `radius_nearest()`.

    vector<node<cv::Point> *> neighbors = tree->radius_nearest(cv::Point(70, 0), 100);

In this example the result is `{(80, 0), (40, 0), (20, 0), (10, 0), (160, 0)}`.

### k-Nearest neighbor search
Use `k_nearest()`.

    vector<node<cv::Point> *> neighbors = tree->k_nearest(cv::Point(70, 0), 5);

In this example the result is `{(80, 0), (40, 0), (20, 0), (10, 0), (160, 0)}`.


## Test
`main.cpp` includes primitive unit tests with `assert`.  
You can run the test by using `make clean run`.


## License
*kdtree* is released under the **MIT License**, see *LICENSE.txt*.
