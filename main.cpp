#include <boost/multiprecision/cpp_int.hpp>

using T = boost::multiprecision::cpp_int;

const int k = 4;
const int dimension = 1<<k;

#include "gadget.h"
#include "rational.h"
#include "optimizer.h"
#include "orbit.h"
#include <vector>
#include <iostream>

int main() {

    std::cout << "Analyzing orbits of the symmetry group..." << std::endl;

    OrbitInfo orbitInfo;
    Optimizer optimizer;
    Evaluator evaluator(orbitInfo);

    std::cout << "Optimizing gadget..." << std::endl;

    auto gadget = optimizer.gadgetSearch(orbitInfo);

    std::cout << "Finished optimizing gadget" << std::endl;
    
    auto randomCost = evaluator.relaxedRandomCost(gadget);
    auto dictCost = Rational<T>(gadget.getTotalWeight(), dimension);

    auto inapproximabilityFactor = randomCost / dictCost;

    std::cout << "The found gadget has inapproximability factor " << inapproximabilityFactor.a << "/" << inapproximabilityFactor.b << std::endl;
}
