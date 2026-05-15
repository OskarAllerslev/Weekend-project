#include "payoff.hpp"
#include <algorithm>

namespace RoughVolatility {

EuropeanCallPayoff::EuropeanCallPayoff(double strike) : m_strike(strike) {}

double EuropeanCallPayoff::operator()(double spot) const {
    auto res = std::max(spot - m_strike, 0.0);
    return res;
}

} // namespace RoughVolatility
