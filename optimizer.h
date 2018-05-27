#include "edge.h"
#include "evaluator.h"
#include "gadget.h"
#include "node.h"
#include "orbit.h"
#include "simplex_rational.h"
#include <iostream>
#include <map>
#include <vector>

class Optimizer {
    public:

    Gadget gadgetSearch(const OrbitInfo& orbitInfo) {

        Evaluator evaluator(orbitInfo);

        using U = Rational<T>;

        auto edgeOrbits = orbitInfo.getAllEdgeOrbits();
        auto nodeOrbits = orbitInfo.getAllNodeOrbits();

        // Construct initial linear program
        std::vector<std::vector<U>> A;
        std::vector<U> b, c;
        
        // The first entries in the vector represent the gadget's weights on different edge orbits
        for (auto orbit : edgeOrbits) {
            c.emplace_back(0);
        }
        // The next entries in the vector represent the gadget's relaxed costs for different Z-assignment orbits
        for (auto orbit : nodeOrbits) {
            c.emplace_back(orbit.size(), 1<<dimension);
        }

        A.emplace_back();
        for (auto orbit : edgeOrbits) {
            A[0].emplace_back(orbit.size());
        }
        for (auto orbit : nodeOrbits) {
            A[0].emplace_back(0);
        }
        b.emplace_back(1);

        A.emplace_back();
        for (auto orbit : edgeOrbits) {
            A[1].emplace_back(-((int)orbit.size()));
        }
        for (auto orbit : nodeOrbits) {
            A[1].emplace_back(0);
        }
        b.emplace_back(-1);

        // Construct initial gadget
        std::map<Edge, T> uniformGadgetWeights;
        for (auto edge : orbitInfo.getAllEdges()) {
            uniformGadgetWeights[edge] = 1;
        }
        Gadget gadget(uniformGadgetWeights);

        std::vector<Gadget> gadgetSet;
        
        // Improve gadget iteratively
        while (find(gadgetSet.begin(), gadgetSet.end(), gadget) == gadgetSet.end()) {
            gadgetSet.push_back(gadget);
            std::cout << "Iteration " << gadgetSet.size() << std::endl;

            // Compute optimal extensions for all orbits of Z-assignments
            for (auto orbit : nodeOrbits) {
                Node representative = orbit[0];
                std::map<Node, bool> assignment;
                for (uint32_t S = 0; S < dimension; S++) {
                    assignment[orbitInfo.chi(S)] = (representative[S] == 1);
                    assignment[-orbitInfo.chi(S)] = !assignment[orbitInfo.chi(S)];
                }

                auto extendedAssignment = evaluator.optimalRelaxedExtension(assignment, gadget);

                // Add constraint corresponding to the extended assignment
                A.emplace_back();
                for (size_t i = 0; i < edgeOrbits.size(); i++) {
                    auto edgeOrbit = edgeOrbits[i];
                    A.back().emplace_back(0);
                    for (auto edge : edgeOrbit) {
                        if (extendedAssignment.getValue(edge.a) != extendedAssignment.getValue(edge.b)) {
                            A.back().back() -= 1;
                        }
                    }
                }
                for (auto orbit2 : nodeOrbits) {
                    A.back().emplace_back(orbit2 == orbit);
                }
                b.emplace_back(0);
            }

            // Solve linear program to find a new gadget
            LPSolver solver(A, b, c);

            std::vector<U> solution;
            solver.solve(solution);

            T commonDenominator = 1;
            for (size_t i = 0; i < edgeOrbits.size(); i++) {
                commonDenominator = lcm(commonDenominator, solution[i].b);
            }

            std::map<Edge, T> gadgetWeights;
            for (size_t i = 0; i < edgeOrbits.size(); i++) {
                assert((solution[i].a * commonDenominator) % solution[i].b == 0);
                T weight = (solution[i].a * commonDenominator) / solution[i].b;
                for (auto edge : edgeOrbits[i]) {
                    gadgetWeights[edge] = weight;
                }
            }
            gadget = Gadget(gadgetWeights);
        }
        
        return gadget;
    }
};
