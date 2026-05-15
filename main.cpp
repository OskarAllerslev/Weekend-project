#include <exception>
#include <iostream>
#include <ostream>
#include <random>
#include <fstream>
#include "fbm_generator.hpp"
#include <chrono>
#include "monte_carlo_pricer.hpp"
void save_prices_to_csv(const std::vector<double>& prices, const std::string& filename) {
    std::ofstream file(filename);
    file << "X\n"; 
    for (double p : prices) {
        file << p << "\n";
    }
    file.close();
    std::cout << "Gemte " << prices.size() << " simuleringer til " << filename << std::endl;
}


int main() 
{

    std::cout << "Testing the fbm generator part" << std::endl;


    int num_sims = 10000;
    int num_steps = 365;
    double T = 1.0;
    double hurst = 0.1;

    double kappa = 5.0;
    double theta = 50.0;
    double x0 = 50.0;

    double v0 = 0.04;
    double nu = 1.5;
    double strike = 55;


    try 
    {
        auto start_time = std::chrono::high_resolution_clock::now();
        std::cout << "Initializing pricer" << std::endl;

        RoughVolatility::MonteCarloPricer pricer(
            num_sims, num_steps, T, hurst, kappa, theta, v0, nu, x0, strike);

        std::cout << "Calculating expected payoff over: " << num_sims << "paths." << std::endl;

        // double price = pricer.run();
        auto all_XT = pricer.run_get_paths();
        save_prices_to_csv(all_XT, "sim_prices.csv");

        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = end_time - start_time;


        // std::cout << "Expected call option price: " << price << std::endl;
        std::cout << "Elapsed time: " << elapsed.count() << std::endl;
        

    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}


