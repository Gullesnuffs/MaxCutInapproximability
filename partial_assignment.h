#pragma once
#include "edge.h"
#include "node.h"
#include <array>
#include <cassert>
#include <map>

class PartialAssignment {
    // assigned[x] specifies if node x is assigned
    std::array<bool, 1LL<<dimension> assigned;

    // value[x] specifies if node x is assigned
    std::array<bool, 1LL<<dimension> value;

    public:

    PartialAssignment(std::map<Node, bool> assignment) {
        for (long long i = 0; i < (1LL<<dimension); i++) {
            assigned[i] = false;
        }
        for (const auto& node : assignment) {
            assigned[node.first.getIndex()] = true;
            value[node.first.getIndex()] = node.second;
        }
    }

    bool isAssigned(const Node& node) const {
        return assigned[node.getIndex()];
    }

    bool getValue(const Node& node) const {
        assert(assigned[node.getIndex()]);
        return value[node.getIndex()];
    }
};
