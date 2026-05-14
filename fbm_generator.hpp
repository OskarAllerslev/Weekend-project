#ifndef FBM_GENERATOR_H
#define FBM_GENERATOR_H

#include <cmath>
#include <vector>
#include <random>
#include <complex>

namespace RoughVolatility {

class FbmGenerator {
public:
    FbmGenerator(double hurst, int num_steps, double T);
    std::vector<double> generate_path(std::mt19937& rng);

private:
    void precompute_eigenvalues();
    double compute_autocovariance(int k) const;
    double single_direct_fourier_transform(int k, const std::vector<double>& c) const;

    double m_hurst;
    int m_num_steps;
    double m_T;
    double m_dt;
    double m_dt_2H;
    std::vector<double> m_eigenvalues;
};

} // namespace RoughVolatility

#endif // FBM_GENERATOR_H
