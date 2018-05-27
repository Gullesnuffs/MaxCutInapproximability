#pragma once
#include "edge.h"
#include "gadget.h"
#include "node.h"
#include "orbit.h"
#include "partial_assignment.h"
#include <cstdint>
#include <future>
#include <lemon/list_graph.h>
#include <lemon/preflow.h>
#include <map>
#include <utility>
#include <vector>

class Evaluator {

    OrbitInfo orbitInfo;

    public:

    Evaluator(const OrbitInfo orbitInfo) : orbitInfo(orbitInfo) {}

    PartialAssignment optimalRelaxedExtension(const PartialAssignment& partialAssignment, const Gadget& gadget) {

        using namespace lemon;

        // Construct nodes in the graph
        ListDigraph g;
        auto source = g.addNode();
        auto sink = g.addNode();
        std::vector<ListDigraph::Node> hypergraphNodes;
        for (long long index = 0; index < (1LL<<dimension); index++) {
            hypergraphNodes.emplace_back(g.addNode());
        }

        // Compute the capacity of each edge in the graph
        ListDigraph::ArcMap<T> capacity(g);
        for (long long index = 0; index < (1LL<<dimension); index++) {
            Node node((uint32_t)index);
            for (unsigned direction = 0; direction < dimension; direction++) {
                auto destination = node.getNeighbour(direction);
                auto arc = g.addArc(
                        hypergraphNodes[node.getIndex()], 
                        hypergraphNodes[destination.getIndex()]);
                T weight = gadget.getWeight(Edge(node, destination));
                capacity[arc] = weight;
            }

            if (partialAssignment.isAssigned(node)) {
                ListDigraph::Arc arc;
                if (partialAssignment.getValue(node)) {
                    arc = g.addArc(
                        source,
                        hypergraphNodes[node.getIndex()]);
                }
                else {
                    arc = g.addArc(
                        hypergraphNodes[node.getIndex()],
                        sink);
                }
                capacity[arc] = gadget.getTotalWeight();
            }
        }

        Preflow<ListDigraph, ListDigraph::ArcMap<T>> preflow(g, capacity, source, sink);
        preflow.runMinCut();

        std::map<Node, bool> assignment;
        for (long long index = 0; index < (1LL<<dimension); index++) {
            Node node((uint32_t)index);
            assignment[Node((uint32_t)index)] = preflow.minCut(hypergraphNodes[index]);
            if (partialAssignment.isAssigned(node)) {
                assert(partialAssignment.getValue(node) == assignment[node]);
            }
        }

        return PartialAssignment(assignment);
    }

    Rational<T> relaxedRandomCost(const Gadget& gadget) {
        T totalValue = 0;
        auto allOrbits = orbitInfo.getAllNodeOrbits();
        for (const auto& orbit : allOrbits) {
            Node representative = orbit[0];
            std::map<Node, bool> assignment;
            for (uint32_t S = 0; S < dimension; S++) {
                assignment[orbitInfo.chi(S)] = (representative[S] == 1);
                assignment[-orbitInfo.chi(S)] = !assignment[orbitInfo.chi(S)];
            }

            auto extendedAssignment = optimalRelaxedExtension(PartialAssignment(assignment), gadget);
            T value = 0;

            for (long long index = 0; index < (1LL<<dimension); index++) {
                Node node((uint32_t)index);
                for (unsigned direction = 0; direction < dimension; direction++) {
                    auto destination = node.getNeighbour(direction);
                    if (extendedAssignment.getValue(node) > extendedAssignment.getValue(destination)) {
                        value += gadget.getWeight(Edge(node, destination));
                    }
                }
            }
            
            totalValue += orbit.size() * value;
        }
        return Rational<T>(totalValue, 1<<dimension);
    }
};
