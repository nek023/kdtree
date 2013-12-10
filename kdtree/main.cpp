//
//  main.cpp
//  kdtree
//
//  Created by Tanaka Katsuma on 2013/12/03.
//  Copyright (c) 2013年 Katsuma Tanaka. All rights reserved.
//

#include <iostream>
#include <vector>
#include <cassert>
#include <set>
#include <ctime> // for time()
#include <cstdlib> // for srand(), rand()

#include "benchmark.hpp"
#include "kdtree.hpp"
#include "point.hpp"

using namespace std;
using namespace kdtree;


#pragma mark - Helper

inline int random(int min, int max) // Example: random(0, 3) => 0, 1, 2
{
    return min + rand() % (max - min);
}

inline pointi random_point(int min, int max)
{
    int x = random(min, max);
    int y = random(min, max);
    
    return pointi(x, y);
}

struct compare_point
{
    bool operator() (const pointi& a, const pointi& b)
    {
        pointi zero = pointi(0, 0);
        double da = zero.distance(a);
        double db = zero.distance(b);
        
        return da < db;
    }
};

vector<pointi> random_points(int size, int min, int max)
{
    set<pointi, compare_point> points;
    
    while (points.size() < size) {
        points.insert(random_point(min, max));
    }
    
    return vector<pointi>(points.begin(), points.end());
}


#pragma mark - Straight Forward Search

pointi sf_nearest(vector<pointi>& points, pointi& query)
{
    assert(points.size() > 0);
    
    pointi nearest_neighbor = points.at(0);
    double min_distance = query.distance(points.at(0));
    
    for (vector<pointi>::iterator it = points.begin(); it != points.end(); ++it) {
        double distance = query.distance(*it);
        
        if (distance < min_distance) {
            nearest_neighbor = *it;
            min_distance = distance;
        }
    }
    
    return nearest_neighbor;
}

vector<pointi> sf_radius_nearest(vector<pointi>& points, pointi& query, double r)
{
    assert(points.size() > 0);
    
    vector<pointi> neighbors;
    
    for (vector<pointi>::iterator p_it = points.begin(); p_it != points.end(); ++p_it) {
        pointi point = *p_it;
        
        if (point.distance(query) <= r) {
            if (neighbors.size() == 0) {
                neighbors.push_back(point);
                continue;
            }
            
            for (vector<pointi>::iterator n_it = neighbors.begin(); n_it <= neighbors.end(); ++n_it) {
                pointi neighbor = *n_it;
                
                if (n_it == neighbors.end()) {
                    neighbors.push_back(point);
                    break;
                }
                
                if (query.distance(point) < query.distance(neighbor)) {
                    neighbors.insert(n_it, point);
                    break;
                }
            }
        }
    }
    
    return neighbors;
}

vector<pointi> sf_k_nearest(vector<pointi>& points, pointi& query, int k)
{
    assert(points.size() > 0);
    
    vector<pointi> neighbors;
    
    for (vector<pointi>::iterator p_it = points.begin(); p_it != points.end(); ++p_it) {
        pointi point = *p_it;
        
        if (neighbors.size() == 0) {
            neighbors.push_back(point);
            continue;
        }
        
        for (vector<pointi>::iterator n_it = neighbors.begin(); n_it <= neighbors.end(); ++n_it) {
            pointi neighbor = *n_it;
            
            if (n_it == neighbors.end()) {
                neighbors.push_back(point);
                break;
            }
            
            if (query.distance(point) < query.distance(neighbor)) {
                neighbors.insert(n_it, point);
                break;
            }
        }
        
        if (neighbors.size() > k) {
            neighbors.pop_back();
        }
    }
    
    return neighbors;
}


#pragma mark - Tests

void test_node()
{
    node<pointi> *node1 = new node<pointi>(pointi(10, 0));
    node<pointi> *node2 = new node<pointi>(pointi(20, 0));
    node<pointi> *node3 = new node<pointi>(pointi(40, 0));
    
    // Check values
    assert(node1->point == pointi(10, 0));
    assert(node2->point == pointi(20, 0));
    assert(node3->point == pointi(40, 0));
    
    // Test is_leaf()
    assert(node1->is_leaf());
    assert(node2->is_leaf());
    assert(node3->is_leaf());
    
    // Test distance()
    assert(node1->distance(node2) == 10.0);
    assert(node2->distance(node1) == 10.0);
    assert(node2->distance(node3) == 20.0);
    assert(node3->distance(node2) == 20.0);
    
    // Test close()
    assert(node1->closer(node2, node3) == node2);
    assert(node2->closer(node1, node3) == node1);
    assert(node3->closer(node1, node2) == node2);
    
    delete node1;
    delete node2;
    delete node3;
}

void test_kdtree()
{
    vector<pointi> points = {
        pointi(10, 10), // 1
        pointi(20, 20), // 2
        pointi(30, 30), // 3
        pointi(40, 40), // 4
        pointi(50, 50)  // 5
    };
    
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    /*
     Estimated tree:
             3
            / \
           2   5
          /   /
         1   4
     */
    
    // The root node of the tree should have left node and right node
    node<pointi> *root = tree->root;
    assert(root->has_left_node());
    assert(root->has_right_node());

    // ... and have the point (30, 30)
    assert(root->point == pointi(30, 30));
    
    // The left node of the root should have left node only
    node<pointi> *left_node = root->left;
    assert(left_node->has_left_node());
    assert(!left_node->has_right_node());
    
    // ... and have the point (20, 20)
    assert(left_node->point == pointi(20, 20));
    
    // The right node of the root should have left node only
    node<pointi> *right_node = root->right;
    assert(right_node->has_left_node());
    assert(!right_node->has_right_node());
    
    // ... and have the point (50, 50)
    assert(right_node->point == pointi(50, 50));
    
    // The left leaf of the tree should have no child node
    node<pointi> *left_leaf = left_node->left;
    assert(left_leaf->is_leaf());
    
    // ... and have the point (10, 10)
    assert(left_leaf->point == pointi(10, 10));
    
    // The right leaf of the tree should have no child node
    node<pointi> *right_leaf = right_node->left;
    assert(right_leaf->is_leaf());
    
    // ... and have the point (40, 40)
    assert(right_leaf->point == pointi(40, 40));
    
    delete root;
}

void test_nearest()
{
    vector<pointi> points = {
        pointi(10, 10),
        pointi(20, 20),
        pointi(40, 40),
        pointi(80, 80),
        pointi(160, 160)
    };
    
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    /*
     Estimated tree:
             3
            / \
           2   5
          /   /
         1   4
     */
    
    node<pointi> *nearest_neighbor = tree->nearest(pointi(50, 50));
    
    // Nearest neighbor should be point (40, 40)
    assert(nearest_neighbor->point == pointi(40, 40));
    
    delete tree;
}

void test_radius_nearest()
{
    vector<pointi> points = {
        pointi(10, 0), // 1
        pointi(20, 0), // 2
        pointi(40, 0), // 3
        pointi(80, 0), // 4
        pointi(160, 0) // 5
    };
    
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    /*
     Estimated tree:
     3
     / \
     2   5
     /   /
     1   4
     */
    
    vector<node<pointi> *> neighbors = tree->radius_nearest(pointi(70, 0), 100);
    
    // Number of neighbors should be five
    assert(neighbors.size() == 5);
    
    // Estimated order of neighbors is: (80, 0), (40, 0), (20, 0), (10, 0), (160, 0)
    assert(neighbors[0]->point == pointi(80, 0));
    assert(neighbors[1]->point == pointi(40, 0));
    assert(neighbors[2]->point == pointi(20, 0));
    assert(neighbors[3]->point == pointi(10, 0));
    assert(neighbors[4]->point == pointi(160, 0));
    
    delete tree;
}

void test_k_nearest()
{
    vector<pointi> points = {
        pointi(10, 0), // 1
        pointi(20, 0), // 2
        pointi(40, 0), // 3
        pointi(80, 0), // 4
        pointi(160, 0) // 5
    };
    
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    /*
     Estimated tree:
             3
            / \
           2   5
          /   /
         1   4
     */
    
    vector<node<pointi> *> neighbors = tree->k_nearest(pointi(70, 0), 5);
    
    // Estimated order of neighbors is: (80, 0), (40, 0), (20, 0), (10, 0), (160, 0)
    assert(neighbors[0]->point == pointi(80, 0));
    assert(neighbors[1]->point == pointi(40, 0));
    assert(neighbors[2]->point == pointi(20, 0));
    assert(neighbors[3]->point == pointi(10, 0));
    assert(neighbors[4]->point == pointi(160, 0));
    
    delete tree;
}


#pragma mark - Random Tests

void test_nearest_random()
{
    vector<pointi> points = random_points(1000, 1, 100);
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    pointi query_point = random_point(1, 100);
    
    // Search nearest neighbor with straight forward method
    pointi sf_nearest_neighbor;
    benchmark("straight-forward") {
        sf_nearest_neighbor = sf_nearest(points, query_point);
    }
    
    // Search nearest neighbor with kdtree
    node<pointi> *nearest_neighbor = NULL;
    benchmark("kdtree") {
        nearest_neighbor = tree->nearest(query_point);
    }
    
    // The distances of results should be equal
    assert(nearest_neighbor->distance(query_point) == sf_nearest_neighbor.distance(query_point));
    
    delete tree;
}

void test_radius_nearest_random()
{
    vector<pointi> points = random_points(1000, 1, 100);
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    pointi query_point = random_point(1, 100);
    double r = 30.0;
    
    // Search k nearest neighbor with straight forward method
    vector<pointi> sf_neighbors;
    benchmark("straight-forward") {
        sf_neighbors = sf_radius_nearest(points, query_point, r);
    }
    
    // Search k nearest neighbor with kdtree
    vector<node<pointi> *> neighbors;
    benchmark("kdtree") {
        neighbors = tree->radius_nearest(query_point, r);
    }
    
    // The size of each vector should be equal
    assert(neighbors.size() == sf_neighbors.size());
    
    // The distances of results should be equal
    for (int i = 0; i < neighbors.size(); i++) {
        assert(neighbors.at(i)->point.distance(query_point) == sf_neighbors.at(i).distance(query_point));
    }
    
    delete tree;
}

void test_k_nearest_random()
{
    vector<pointi> points = random_points(1000, 1, 100);
    kdtree<pointi> *tree = new kdtree<pointi>(points);
    
    pointi query_point = random_point(1, 100);
    int k = random(5, 20);
    
    // Search k nearest neighbor with straight forward method
    vector<pointi> sf_neighbors;
    benchmark("straight-forward") {
        sf_neighbors = sf_k_nearest(points, query_point, k);
    }
    
    // Search k nearest neighbor with kdtree
    vector<node<pointi> *> neighbors;
    benchmark("kdtree") {
        neighbors = tree->k_nearest(query_point, k);
    }
    
    // The size of each vector should be equal
    assert(neighbors.size() == sf_neighbors.size());
    
    // The distances of results should be equal
    for (int i = 0; i < neighbors.size(); i++) {
        assert(neighbors.at(i)->point.distance(query_point) == sf_neighbors.at(i).distance(query_point));
    }
    
    delete tree;
}


#pragma mark - Main

int main(int argc, const char * argv[])
{
    // Setup
    srand((unsigned int)time(NULL));
    
    // Tests
    test_node();
    test_kdtree();
    test_nearest();
    test_radius_nearest();
    test_k_nearest();
    
    // Random Tests
    test_nearest_random();
    test_radius_nearest_random();
    test_k_nearest_random();
    
    return 0;
}
