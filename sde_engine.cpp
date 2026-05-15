#include "sde_engine.hpp"
#include <cmath>
#include <iostream>
#include <random>
#include <vector>

namespace RoughVolatility {

SdeEngine::SdeEngine(double kappa, double theta, double v0, double nu, double x0, double hurst, double T, int num_steps)
    : m_kappa(kappa), m_theta(theta), m_v0(v0), m_nu(nu), m_x0(x0), m_hurst(hurst), m_T(T), m_num_steps(num_steps) {}

double SdeEngine::simulate_path(const std::vector<double>& fbm_path, std::mt19937& rng) {
    /*
    we want to calculate
    X_{i+1} = X_i + \kappa(\theta - X_i)dt + \sqrt{V_i} X_i \sqrt{dt}Z_{i+1}
    see the readme
    */
    double dt = m_T / m_num_steps;
    double sqrt_dt = std::sqrt(dt);
    double X_t = m_x0; 

    std::normal_distribution<double> standard_normal(0.0,1.0);

    // the loop 
    for (int i = 0; i < m_num_steps; i++)
    {
        double t_i = i * dt;
        double t_i_2h = std::pow(t_i, 2.0*m_hurst);
        double V_i = m_v0 * std::exp(m_nu * fbm_path[i] - 0.5 * m_nu * m_nu * t_i_2h);
        double drift = m_kappa * ( m_theta - X_t) * dt;
        double Z = standard_normal(rng);
        double diffusion = std::sqrt(V_i) * X_t * sqrt_dt * Z;
        X_t = X_t + drift + diffusion;
    }
    return X_t;
}

} // namespace RoughVolatility
