#include <exception>
#include <iostream>
#include <ostream>
#include <random>
#include <fstream>
#include <chrono>
#include <vector>
#include "fbm_generator.hpp"
#include "monte_carlo_pricer.hpp"

int main() 
{
    std::cout << "Generating prices" << std::endl;

    int num_sims = 5000;
    int num_steps = 365; 
    double hurst = 0.1;

    // OU params
    double kappa = 5.0;
    double theta = 50.0;
    double x0 = 50.0;

    // Vol params 
    double v0 = 0.04;
    double nu = 1.5;

    // vol surface grid
    std::vector<double> maturities = {0.1, 0.25, 0.5, 0.75, 1.0, 2.0};
    std::vector<double> strikes = {40, 45, 50, 55, 60};

    try 
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        
        std::ofstream file("surface_prices.csv");
        file << "T,K,Price\n"; 

        int total_calcs = maturities.size() * strikes.size();
        int current_calc = 1;

        for (double current_T : maturities) 
        {
            for (double current_K : strikes)
            {
                std::cout << "Beregner [" << current_calc << "/" << total_calcs 
                          << "] - T: " << current_T << ", K: " << current_K << "..." << std::flush;

                RoughVolatility::MonteCarloPricer pricer(
                    num_sims, num_steps, current_T, hurst, kappa, theta, v0, nu, x0, current_K);

                double price = pricer.run();

                file << current_T << "," << current_K << "," << price << "\n";
                
                std::cout << " Pris: " << price << std::endl;
                current_calc++;
            }
        }

        file.close(); 

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;

        std::cout << "\nSucces! Alle priser gemt i 'surface_prices.csv'." << std::endl;
        std::cout << "Samlet tid: " << elapsed.count() << " sekunder." << std::endl;

    }
    catch (const std::exception& e)
    {
        std::cerr << "Fejl: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}