using T = long long;

const int k = 4;
const int dimension = 1<<k;

#include "evaluator.h"
#include "node.h"
#include "orbit.h"
#include <vector>
#include <iostream>

int main() {

    std::cout << "Analyzing orbits of the symmetry group..." << std::endl;

    OrbitInfo orbitInfo;

    Node Z(0b0000000000000000);
    Node A(0b0000000000000001);
    Node B(0b0000000000000011);
    Node C(0b0000000000000111);
    Node D(0b0000000000001111);
    Node E(0b0000000000010111);
    Node F(0b0000000100010111);
    Node G(0b1000000100010111);

    Edge ZA(Z, A);
    Edge AB(A, B);
    Edge BC(B, C);
    Edge CD(C, D);
    Edge CE(C, E);
    Edge EF(E, F);
    Edge FG(F, G);
    
    auto orbitZA = orbitInfo.getOrbit(ZA);
    auto orbitAB = orbitInfo.getOrbit(AB);
    auto orbitBC = orbitInfo.getOrbit(BC);
    auto orbitCD = orbitInfo.getOrbit(CD);
    auto orbitCE = orbitInfo.getOrbit(CE);
    auto orbitEF = orbitInfo.getOrbit(EF);
    auto orbitFG = orbitInfo.getOrbit(FG);

    std::map<Edge, T> weight;

    auto setWeightsInOrbit = [&weight](const std::vector<Edge>& orbit, T w) {
        for (const Edge& edge : orbit) {
            weight[edge] = w;
        }
    };
    
    std::cout << "Constructing gadget..." << std::endl;

    setWeightsInOrbit(orbitZA, 4389840);
    setWeightsInOrbit(orbitAB, 292656);
    setWeightsInOrbit(orbitBC, 54610);
    setWeightsInOrbit(orbitCD, 102042);
    setWeightsInOrbit(orbitCE, 10488);
    setWeightsInOrbit(orbitEF, 1995);
    setWeightsInOrbit(orbitFG, 0);

    Gadget gadget(weight);

    Evaluator evaluator(orbitInfo);
    
    std::cout << "Evaluating gadget..." << std::endl;

    auto value = evaluator.evaluateGadget(gadget);

    std::cout << "Gadget value: " << value.first << "/" << value.second << std::endl;
    
}
