#pragma once
#include <cstdint>
#include <cstdlib>

class Node {
    uint32_t x;

    public:
    
    Node() {
        static_assert(dimension <= 32, "Hypercube dimensions greater than 32 aren't supported because nodes are represented using only 32 bits");
        x = 0;
    }

    Node(uint32_t x) : x(x) {
        static_assert(dimension <= 32, "Hypercube dimensions greater than 32 aren't supported because nodes are represented using only 32 bits");
    }

    Node(const Node& other) {
        x = other.x;
    }

    Node getNeighbour(unsigned direction) const {
        return Node(getIndex()^(1u<<direction));
    }

    uint32_t getIndex() const {
        return x;
    }

    int operator[](int index) const {
        if (x&(1u<<index)) {
            return -1;
        }
        else {
            return 1;
        }
    }

    Node operator-() const {
        uint32_t inv = uint32_t((1LL<<dimension) - 1 - x);
        return Node(inv);
    }

    Node operator^(const Node& other) const {
        return Node(x ^ other.x);
    }

    Node& operator^=(const Node& other) {
        x ^= other.x;
        return *this;
    }

    bool operator<(const Node& other) const {
        return x < other.x;
    }
    
    bool operator==(const Node& other) const {
        return x == other.x;
    }
};
