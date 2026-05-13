#include "payoff.h"
#include <iostream>

namespace RoughVolatility {

EuropeanCallPayoff::EuropeanCallPayoff(double strike) : m_strike(strike) {}

double EuropeanCallPayoff::operator()(double spot) const {
    std::cout << "Warning: Payoff calculation not implemented." << std::endl;
    return 0.0;
}

} // namespace RoughVolatility
