#include "fbm_generator.hpp"
#include <cstdlib>
#include <iostream>
#include <pplwin.h>

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

double FbmGenerator::compute_autocovariance(int k) const
{
    /*
    \gamma(k) = 1/2 \deltat^{2h} \left( |k+1|^2H - 2|k|^2H + |k-1|^2H \right)
    */
    if (k==0)
    {
        return m_dt_squared;
    }

    double first_part = std::pow(std::abs( k+1.0), 2.0 * m_hurst);
    double second_part = std::pow(std::abs( (double)k), 2.0 * m_hurst);
    double third_part = std::pow(std::abs( k-1.0), 2.0 * m_hurst);

    return 0.5 * m_dt_squared * ( first_part - 2*second_part + third_part);
}

} // namespace RoughVolatility
