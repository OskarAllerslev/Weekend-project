#include <exception>
#include <iostream>
#include <ostream>
#include <random>
#include <fstream>
#include "fbm_generator.hpp"
#include "monte_carlo_pricer.hpp"

void save_to_csv(const std::vector<double>& path, const std::string& filename) {
    std::ofstream file(filename);
    file << "step,value\n"; 
    for (size_t i = 0; i < path.size(); ++i) {
        file << i << "," << path[i] << "\n";
    }
    file.close();
    std::cout << "Sti gemt i " << filename << std::endl;
}


int main() 
{

    std::cout << "Testing the fbm generator part" << std::endl;


    double H = 0.01;
    double num_steps = 10000; // N
    double T = 2.0; // eks one year


    try 
    {
        std::cout << "Initializing the FbmGenerator (H=" << H << ", N=" << num_steps << ", T=" << T << std::endl;

        RoughVolatility::FbmGenerator fbm_gen(H, num_steps, T);


        std::random_device rd;
        std::mt19937 rng(rd());
        std::vector<double> path = fbm_gen.generate_path(rng);
        
        save_to_csv(path, "fbm_path");

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


