#pragma once
#include "node.h"
#include <algorithm>
#include <utility>

class Edge {
    
    public:

    Node a, b;

    Edge() : a(), b() {}

    Edge(Node a, Node b) : a(a), b(b) {}

    // Define less than operator to allow storing edges in ordered trees
    bool operator<(const Edge& other) const {

        // Since the edges are unordered we first sort the nodes in each edge
        auto myNodes = std::make_pair(a.getIndex(), b.getIndex());
        if (myNodes.second < myNodes.first)
            std::swap(myNodes.first, myNodes.second);
        auto otherNodes = std::make_pair(other.a.getIndex(), other.b.getIndex());
        if (otherNodes.second < otherNodes.first)
            std::swap(otherNodes.first, otherNodes.second);

        // Compare lexicographically
        return myNodes < otherNodes;
    }
};
