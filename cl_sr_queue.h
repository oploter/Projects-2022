#ifndef CL_QUEUE_H
#define CL_QUEUE_H

#include <queue>
#include <utility>

inline std::queue<std::pair<int, std::pair<int, int>>>& queue_for_ser(){
    static std::queue<std::pair<int, std::pair<int, int>>> q;
    return q;
}

inline std::queue<std::pair<int, std::pair<int, int>>>& queue_for_cl(){
    static std::queue<std::pair<int, std::pair<int, int>>> q;
    return q;
}

#endif