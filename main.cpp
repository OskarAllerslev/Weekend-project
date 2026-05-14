#include <exception>
#include <iostream>
#include <ostream>
#include <random>
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


        std::random_device rd;
        std::mt19937 rng(rd());
        std::vector<double> path = fbm_gen.generate_path(rng);

        std::cout << "Suceces: path generated with " << path.size() << " points." << std::endl;
        std::cout << "The first five steps are: " << std::endl;

        for (int i = 0; i < 6; i++)
        {
            std::cout << "W^H(" << i << ") = " << path[i] << std::endl;
        }
        std::cout << " \n" << std::endl;

        std::cout << "Terminal value: " << path.back() << std::endl;




    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;

}
