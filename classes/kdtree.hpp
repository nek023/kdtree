//
//  kdtree.h
//  kdtree
//
//  Created by Tanaka Katsuma on 2013/12/03.
//  Copyright (c) 2013年 Katsuma Tanaka. All rights reserved.
//

#ifndef __kdtree__kdtree__
#define __kdtree__kdtree__

#include <iostream>
#include "node.hpp"

namespace kdtree {
    template <typename T>
    class kdtree {
    public:
        node<T> *root;
        
        ///-----------------------------------------------------------------------
        /// @name Constructor
        ///-----------------------------------------------------------------------
        /**
         *  Initialize kdtree.
         *
         *  @param points  A vector of points.
         *
         *  @return Initialized kdtree instance.
         */
        kdtree(std::vector<T> points) : kdtree<T>(&points[0], (int)points.size()) {}
        
        /**
         *  Initialize kdtree.
         *
         *  @param points  An array of points.
         *  @param size    A size of the array.
         *
         *  @return Initialized kdtree instance.
         */
        kdtree(T *points, int size) {
            this->root = new node<T>(points, size);
        }
        
        ///-----------------------------------------------------------------------
        /// @name Destructor
        ///-----------------------------------------------------------------------
        /**
         *  Delete kdtree object.
         */
        ~kdtree() {
            delete this->root;
        }
        
        ///-----------------------------------------------------------------------
        /// @name Nearest Neighbor Search
        ///-----------------------------------------------------------------------
        /**
         *  Search for the nearest neighbor in the tree.
         *
         *  @param query_point  A query point.
         *
         *  @return The nearest neighbor node.
         */
        node<T> * nearest(T query_point) {
            return this->root->nearest(query_point);
        }
        
        /**
         *  Search for the nearest neighbor in the tree.
         *
         *  @param query  A query node.
         *
         *  @return The nearest neighbor node.
         */
        node<T> * nearest(node<T> *query) {
            return this->root->nearest(query);
        }
        
        /**
         *  Search for all nearest neighbors within a certain radius of a point.
         *
         *  @param query_point  A query point.
         *  @param r            A radius of the circle.
         *
         *  @return The vector of neighbors.
         */
        std::vector<node<T> *> radius_nearest(T query_point, const double r) {
            return this->root->radius_nearest(query_point, r);
        }
        
        /**
         *  Search for all nearest neighbors within a certain radius of a point.
         *
         *  @param query  A query point.
         *  @param r      A radius of the circle.
         *
         *  @return The vector of neighbors.
         */
        std::vector<node<T> *> radius_nearest(node<T> *query, const double r) {
            return this->root->radius_nearest(query, r);
        }
        
        ///-----------------------------------------------------------------------
        /// @name k-Nearest Neighbor Search
        ///-----------------------------------------------------------------------
        /**
         *  Search for k-nearest neighbors in the tree.
         *
         *  @param query_point  A query point.
         *  @param k            Number of closest points to find.
         *
         *  @return The vector of neighbors.
         */
        std::vector<node<T> *> k_nearest(T query_point, const int k) {
            return this->root->k_nearest(query_point, k);
        }
        
        /**
         *  Search for k-nearest neighbors in the tree.
         *
         *  @param query  A query node.
         *  @param k      Number of closest points to find.
         *
         *  @return The vector of neighbors.
         */
        std::vector<node<T> *> k_nearest(node<T> *query, const int k) {
            return this->root->k_nearest(query, k);
        }
    };
}

#endif /* defined(__kdtree__kdtree__) */
