#ifndef MONTE_CARLO_PRICER_H
#define MONTE_CARLO_PRICER_H

#include "sde_engine.hpp"
#include "fbm_generator.hpp"
#include "payoff.hpp"
#include <random>

namespace RoughVolatility {

class MonteCarloPricer {
public:
    MonteCarloPricer(
        int num_sims,
        int num_steps,
        double T,
        double hurst,
        double kappa,
        double theta,
        double v0,
        double nu,
        double x0,
        double strike
    );

    double run();

private:
    int m_num_sims;
    int m_num_steps;
    double m_T;
    
    FbmGenerator m_fbm_generator;
    SdeEngine m_sde_engine;
    EuropeanCallPayoff m_payoff;
};

} // namespace RoughVolatility

#endif // MONTE_CARLO_PRICER_H
