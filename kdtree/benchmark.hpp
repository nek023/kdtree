//
//  benchmark.h
//  kdtree
//
//  Created by Tanaka Katsuma on 2013/12/06.
//  Copyright (c) 2013年 Katsuma Tanaka. All rights reserved.
//

#ifndef __kdtree__benchmark__
#define __kdtree__benchmark__

#include <iostream>
#include <sys/time.h>

struct __bench__
{
    double start;
    
    __bench__(int dummy) {
        start = time();
    }
    
    ~__bench__() {
        fprintf(stderr, ": %.6f sec\n", time() - start);
    }
    
    double time() {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        return tv.tv_sec + tv.tv_usec * 1e-6;
    }
    
    operator bool () {
        return false;
    }
};

#define benchmark(...) \
if (__bench__ __b__ = __bench__(fprintf(stderr, __VA_ARGS__))); else

#endif /* defined(__kdtree__benchmark__) */
