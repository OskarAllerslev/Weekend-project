#include "fbm_generator.hpp"
#include <cmath>
#include <complex>
#include <cstdlib>
#include <iostream>
#include <pplwin.h>
#include <random>
#include <stdexcept>
#include <vector>

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

std::vector<double> FbmGenerator::generate_path(std::mt19937 &rng) 
{
    int N = m_num_steps;
    int M = 2 * N;

    // generator complex standard normal rv. M 

    // first random variables
    std::normal_distribution<double> dist(0.0, 1.0);
    std::vector<double> Z(M, 0.0);

    for (int i = 0; i <= M - 1; i++)
    {
        Z[i] = dist(rng);
    }

    // generate the complex variables

    std::vector<std::complex<double>> V(M, 0.0);

    V[0] = std::complex<double>(Z[0], 0);
    V[N] = std::complex<double>(Z[N], 0);
    for (int i = 1; i < N; i++) {
      double sqrt_2 = std::sqrt(2.0);
      V[i] = std::complex<double>(Z[i], Z[M-i]) / sqrt_2;
      V[M-i] = std::complex<double>(Z[i], -Z[M-i]) / sqrt_2;
    }


    // scaling and inverse discrete fourier transform
    const double pi = std::acos(-1.0);
    std::vector<double> fgn_increments(N, 0.0);

    for (int j = 0; j <= N-1; j++)
    {
        double sum_real = 0.0;
        for (int k = 0; k <= M -1; k++)
        {
            /*
            sqrt{g_k } * V* exp{2 \pi j k/M}
            */
            std::complex<double> U_k = std::sqrt(m_eigenvalues[k] )* V[k];
            double theta = (2.0 * pi * j * k )/M;
            std::complex<double> exp_ik = std::complex<double> (std::cos(theta), std::sin(theta));

            std::complex<double> product = U_k * exp_ik;

            // we only need the real part - see test_for_hurst_effect.pdf
            sum_real += product.real();
        }
        fgn_increments[j] = sum_real / std::sqrt(M);
    }
    // cumulative sum to build the path
    
    std::vector<double> path(N + 1, 0.0);
    path[0] = 0.0; // according to the definition of a levy process and also fbm
    for (int i =0; i <= N-1; i++)
    {
        path[1+i] = path[i] + fgn_increments[i];
    }
    return path;

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
