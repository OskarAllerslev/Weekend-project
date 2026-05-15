#include "monte_carlo_pricer.hpp"
#include "fbm_generator.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <omp.h>

namespace RoughVolatility {

// constructor 
MonteCarloPricer::MonteCarloPricer(
    int num_sims, int num_steps, double T, double hurst,
    double kappa, double theta, double v0, double nu, double x0, double strike
) : m_num_sims(num_sims),
    m_num_steps(num_steps),
    m_T(T),
    m_fbm_generator(hurst, num_steps, T),
    m_sde_engine(kappa, theta, v0, nu, x0, hurst, T, num_steps),
    m_payoff(strike)
    // m_rng(std::random_device{}()) // remove due to running parallel
{}

double MonteCarloPricer::run() {
    
    double sum_payoff = 0.0;

    #pragma omp parallel for reduction(+:sum_payoff)
    for (int i = 0; i < m_num_sims; i++)
    {
        std::mt19937 local_rng(42 + i);

        std::vector<double> fbm_path = m_fbm_generator.generate_path(local_rng);
        double X_T = m_sde_engine.simulate_path(fbm_path,local_rng);
        sum_payoff += m_payoff(X_T);
    }
    return sum_payoff / m_num_sims; // expectation of the samples 
}

std::vector<double> MonteCarloPricer::run_get_paths() 
{
    std::vector<double> terminal_prices(m_num_sims, 0.0);
    #pragma omp parallel for 
    for (int i = 0; i < m_num_sims; i++)
    {
        std::mt19937 local_rng(42 + i);

        std::vector<double> fbm_path = m_fbm_generator.generate_path(local_rng);
        double X_T = m_sde_engine.simulate_path(fbm_path,local_rng);

        terminal_prices[i] = X_T;
    }
    return terminal_prices;
}

} // namespace RoughVolatility
