#include "fbm_generator.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <pplwin.h>
#include <stdexcept>

namespace RoughVolatility {

// constructor
FbmGenerator::FbmGenerator(double hurst, int num_steps, double T)
    : m_hurst(hurst), m_num_steps(num_steps), m_T(T) {
  m_dt = m_T / m_num_steps;
  m_dt_2H = std::pow(m_dt, 2.0 * m_hurst);

  std::cout << "FbmGenerator: Pre-computing eigenvalues..." << std::endl;
  precompute_eigenvalues();
  std::cout << "FbmGenerator: ...Done." << std::endl;
}

void FbmGenerator::precompute_eigenvalues() 
{
    int N = m_num_steps;

    std::vector<double> c(2*N, 0.0);

    // we calculate for the first half 
    for (int j = 0; j <= N; j++)
    {
        c[j] = FbmGenerator::compute_autocovariance(j);
    }
    // we mirror for the second half
    for (int j = 1; j < N; j++)
    {
        // we start from the max index and go down
        c[2 * N - j] = c[j];
    }

    // we calculate the actual eigenvalues here
    // using the direct fourier transform method we have 
    m_eigenvalues.assign(2*N, 0.0);
    for (int k = 0; k < 2*N; k++)
    {
        m_eigenvalues[k] = single_direct_fourier_transform(k, c);
        // we need to ensure that g_k >= 0 \forall k 
        if (m_eigenvalues[k] < 0.0) 
        {
            throw std::runtime_error("David-Harte algorithm condition g_k >= 0 detected");
        }
    }

}

std::vector<double> FbmGenerator::generate_path(std::mt19937 &rng) {
  std::cout << "Warning: FbmGenerator::generate_path() not implemented."
            << std::endl;
  return std::vector<double>(m_num_steps + 1, 0.0);
}

double FbmGenerator::compute_autocovariance(int k) const {
  /*
  \gamma(k) = 1/2 \deltat^{2h} \left( |k+1|^2H - 2|k|^2H + |k-1|^2H \right)
  */
  if (k == 0) {
    return m_dt_2H;
  }

  double first_part = std::pow(std::abs(k + 1.0), 2.0 * m_hurst);
  double second_part = std::pow(std::abs((double)k), 2.0 * m_hurst);
  double third_part = std::pow(std::abs(k - 1.0), 2.0 * m_hurst);
  return 0.5 * m_dt_2H * (first_part - 2 * second_part + third_part);
}


double FbmGenerator::single_direct_fourier_transform(int k, const std::vector<double>& c) const
{
    /*
    g_k = \sum_{j=0}^{2N-1} c_j cos(2pi * j * k / 2N) denote 2N = M c_j is the autocov for j
    */
    double g_k = 0.0;
    int M = c.size();
    const double pi = std::acos(-1.0); // pi

    for (int j = 0; j <= M - 1; j++)
    {
        g_k += c[j] * std::cos(2.0 * pi * k*j / M);
    }
    return g_k;
}



} // namespace RoughVolatility
