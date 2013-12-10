//
//  point.h
//  kdtree
//
//  Created by Tanaka Katsuma on 2013/12/06.
//  Copyright (c) 2013年 Katsuma Tanaka. All rights reserved.
//

#ifndef __kdtree__point__
#define __kdtree__point__

#include <iostream>
#include <cmath>

template <typename T>
class point {
public:
    T x, y;
    
    point() : x(0), y(0) {};
    point(T x, T y) : x(x), y(y) {};
    point(const point& p) : x(p.x), y(p.y) {};
    
    inline double distance(const point& p) {
        double dx = this->x - p.x;
        double dy = this->y - p.y;
        
        return std::sqrt(dx * dx + dy * dy);
    }
    
    bool operator == (const point& obj) {
        return (this->x == obj.x && this->y == obj.y);
    }
    
    bool operator != (const point& obj) {
        return (this->x != obj.x || this->y != obj.y);
    }
    
    friend inline std::ostream& operator << (std::ostream& os, const point& obj) {
        os << "(" << obj.x << ", " << obj.y << ")";
        return os;
    }
};

typedef point<int> pointi;

#endif /* defined(__kdtree__point__) */
