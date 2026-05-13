#include "monte_carlo_pricer.h"
#include <iostream>
#include <vector>

namespace RoughVolatility {

MonteCarloPricer::MonteCarloPricer(
    int num_sims, int num_steps, double T, double hurst,
    double kappa, double theta, double v0, double nu, double x0, double strike
) : m_num_sims(num_sims),
    m_num_steps(num_steps),
    m_T(T),
    m_fbm_generator(hurst, num_steps, T),
    m_sde_engine(kappa, theta, v0, nu, x0, hurst, T, num_steps),
    m_payoff(strike)
{}

double MonteCarloPricer::run() {
    std::cout << "Warning: MonteCarloPricer::run() is a stub." << std::endl;
    double total_payoff = 0.0;

    for (int i = 0; i < m_num_sims; ++i) {
        std::vector<double> fbm_path = m_fbm_generator.generate_path();
        double final_price = m_sde_engine.simulate_path(fbm_path);
        total_payoff += m_payoff(final_price);
    }
    
    std::cout << "Warning: Result calculation (averaging, discounting) not implemented." << std::endl;
    return 0.0;
}

} // namespace RoughVolatility
