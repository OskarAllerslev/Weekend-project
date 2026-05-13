#include "sde_engine.h"
#include <iostream>

namespace RoughVolatility {

SdeEngine::SdeEngine(double kappa, double theta, double v0, double nu, double x0)
    : m_kappa(kappa), m_theta(theta), m_v0(v0), m_nu(nu), m_x0(x0) {}

double SdeEngine::simulate_path(const std::vector<double>& fbm_path, double T, int num_steps) {
    std::cout << "Warning: SdeEngine::simulate_path() not implemented." << std::endl;
    return m_x0;
}

} // namespace RoughVolatility
