#include <iostream>
#include "monte_carlo_pricer.h"

int main() 
{
    // Simulation parameters (dummy values)
    const int num_sims = 10;
    const int num_steps = 100;
    const double T = 1.0;
    const double H = 0.1;
    const double kappa = 3.0;
    const double theta = 0.05;
    const double v0 = 0.02;
    const double nu = 0.3;
    const double x0 = 100.0;
    const double strike = 100.0;

    std::cout << "Initializing Monte Carlo Pricer..." << std::endl;
    RoughVolatility::MonteCarloPricer pricer(
        num_sims, num_steps, T, H, kappa, theta, v0, nu, x0, strike
    );

    std::cout << "Running Monte Carlo simulation..." << std::endl;
    double price = pricer.run();
    std::cout << "Simulation finished." << std::endl;

    std::cout << "Calculated option price (stub): " << price << std::endl;

    return 0;
}
