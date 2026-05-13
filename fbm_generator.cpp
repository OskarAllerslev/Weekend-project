#include "fbm_generator.hpp"
#include <iostream>

namespace RoughVolatility {

FbmGenerator::FbmGenerator(double hurst, int num_steps, double T)
    : m_hurst(hurst), m_num_steps(num_steps), m_T(T) 
{
    std::cout << "FbmGenerator: Pre-computing eigenvalues..." << std::endl;
    precompute_eigenvalues();
    std::cout << "FbmGenerator: ...Done." << std::endl;
}

void FbmGenerator::precompute_eigenvalues() {
    std::cout << "Warning: Davies-Harte eigenvalue computation not implemented." << std::endl;
    m_eigenvalues.assign(2 * m_num_steps, 1.0);
}

std::vector<double> FbmGenerator::generate_path(std::mt19937& rng) {
    std::cout << "Warning: FbmGenerator::generate_path() not implemented." << std::endl;
    return std::vector<double>(m_num_steps + 1, 0.0);
}

double compute_autocovariance(int k)
{
    // tmp

    return 0.0;
}

} // namespace RoughVolatility
