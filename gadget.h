#pragma once
#include "edge.h"
#include <cassert>
#include <map>

class Gadget {

    std::map<Edge, T> weight;
    T totalWeight;

    public:

    Gadget(std::map<Edge, T> weight) : weight(weight) {
        totalWeight = 0;
        for (const auto& it : weight) {
            totalWeight += it.second;
        }
    }

    T getWeight(const Edge& edge) const {
        auto it = weight.find(edge);
        assert(it != weight.end());
        return it->second;
    }

    T getTotalWeight() const {
        return totalWeight;
    }
};
