#pragma once
#include "edge.h"
#include "node.h"
#include <cstdint>
#include <map>
#include <set>
#include <vector>

class OrbitInfo {

    uint8_t numNodeTypes;
    std::vector<uint8_t> nodeType;

    void splitBasedOnType(int checkType) {

        // Computes how many neighbours of the checked type a given node has
        auto numberOfNeighboursOfType = [checkType, this](uint32_t node) {
            int cnt = 0;
            for (int j = 0; j < dimension; j++) {
                uint32_t neighbour = node^(1u<<j);
                if (nodeType[neighbour] == checkType) {
                    cnt++;
                }
            }
            return cnt;
        };

        // Compute how many neighbours of the checked type that nodes of different types have
        std::vector<std::set<int>> seenNeighbourCounts(numNodeTypes);
        for (size_t i = 0; i < nodeType.size(); i++) {
            seenNeighbourCounts[nodeType[i]].insert(numberOfNeighboursOfType((uint32_t)i));
        }

        // Compute what node type a node should get based on how many neighbours it has of the checked type
        std::vector<std::map<int, uint8_t>> countToNewNodeType(numNodeTypes);
        for (size_t type = 0; type < countToNewNodeType.size(); type++) {
            auto it = seenNeighbourCounts[type].begin();
            countToNewNodeType[type][*it] = (int8_t)type;
            for (it++; it != seenNeighbourCounts[type].end(); it++) {
                countToNewNodeType[type][*it] = numNodeTypes;
                numNodeTypes++;
            }
        }

        std::vector<uint8_t> newNodeType(nodeType.size());

        // Update the node types
        for (size_t i = 0; i < nodeType.size(); i++) {
            newNodeType[i] = countToNewNodeType[nodeType[i]][numberOfNeighboursOfType((uint32_t)i)];
        }

        nodeType = move(newNodeType);
    }

    public:

    Node chi (uint32_t S) {
        uint32_t z = 0;
        for (unsigned i = 0; i < k; i++) {
            if (S&(1<<i)) {
                for (unsigned j = 0; j < dimension; j++) {
                    if (j&(1<<i)) {
                        z ^= 1u<<j;
                    }
                }
            }
        }
        return Node(z);
    }

    OrbitInfo() {
        // Start by splitting the set of nodes based on whether they belong to Z
        numNodeTypes = 2;
        nodeType = std::vector<uint8_t>(1LL<<dimension, 1);
        for (uint32_t S = 0; S < dimension; S++) {
            nodeType[chi(S).getIndex()] = 0;
            nodeType[(-chi(S)).getIndex()] = 0;
        }

        for (int checkType = 0; checkType < numNodeTypes; ++checkType) {
            // Split sets of nodes based on how many neighbours they have of type checkType
            splitBasedOnType(checkType);
        }
    }

    std::vector<std::vector<Node>> getAllOrbits() {
        std::vector<std::vector<Node>> allOrbits(numNodeTypes);
        for (size_t node = 0; node < nodeType.size(); node++) {
            allOrbits[nodeType[node]].emplace_back(node);
        }
        return allOrbits;
    }

    std::vector<Node> getOrbit(const Node& representative) {
        uint8_t type = nodeType[representative.getIndex()];

        std::vector<Node> orbit;
        for (size_t node = 0; node < nodeType.size(); node++) {
            if (nodeType[node] == type) {
                orbit.emplace_back(node);
            }
        }

        return orbit;
    }

    std::vector<Edge> getOrbit(const Edge& representative) {
        uint8_t atype = nodeType[representative.a.getIndex()];
        uint8_t btype = nodeType[representative.b.getIndex()];

        std::vector<Edge> orbit;
        for (size_t node = 0; node < nodeType.size(); node++) {
            if (nodeType[node] != atype) {
                continue;
            }
            Node a((uint32_t)node);
            for (int j = 0; j < dimension; j++) {
                Node b = a.getNeighbour(j);
                if (nodeType[b.getIndex()] == btype) {
                    orbit.emplace_back(a, b);
                }
            }
        }

        return orbit;
    }
};
