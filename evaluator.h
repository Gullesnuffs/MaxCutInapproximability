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

	T getValue(const PartialAssignment& partialAssignment, const Gadget& gadget) {

        using namespace lemon;

        ListDigraph g;
        auto source = g.addNode();
        auto sink = g.addNode();
        std::vector<ListDigraph::Node> hypergraphNodes;

        for (long long index = 0; index < (1LL<<dimension); index++) {
            hypergraphNodes.emplace_back(g.addNode());
        }

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
        preflow.run();

        return preflow.flowValue();
    }

    public:

    Evaluator(const OrbitInfo orbitInfo) : orbitInfo(orbitInfo) {}

    std::pair<T, T> evaluateGadget(const Gadget& gadget) {

        T totalValue = 0;
        T dictatorValue = 0;

        std::map<uint32_t, T> orbitValue;
        auto allOrbits = orbitInfo.getAllOrbits();
        for (const auto& orbit : allOrbits) {
            Node representative = orbit[0];
            std::map<Node, bool> assignment;
            for (uint32_t S = 0; S < dimension; S++) {
                assignment[orbitInfo.chi(S)] = (representative[S] == 1);
                assignment[-orbitInfo.chi(S)] = !assignment[orbitInfo.chi(S)];
            }

            T value = getValue(PartialAssignment(assignment), gadget);
            totalValue += orbit.size() * value;
            orbitValue[orbit[0].getIndex()] = value;

            if (representative == Node(0)) {
                dictatorValue = value;
            }
        }

        return std::make_pair(totalValue, dictatorValue * (1LL<<dimension));
    }
};
