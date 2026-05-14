#include <exception>
#include <iostream>
#include "fbm_generator.hpp"
#include "monte_carlo_pricer.hpp"

int main() 
{

    std::cout << "Testing the fbm generator part" << std::endl;


    double H = 0.1;
    double num_steps = 1000; // N
    double T = 1.0; // eks one year


    try 
    {
        std::cout << "Initializing the FbmGenerator (H=" << H << ", N=" << num_steps << ", T=" << T << std::endl;

        RoughVolatility::FbmGenerator fbm_gen(H, num_steps, T);

        std::cout << "Succes" << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}
