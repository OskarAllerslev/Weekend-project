#ifndef FBM_GENERATOR_H
#define FBM_GENERATOR_H

#include <cmath>
#include <vector>
#include <random>

namespace RoughVolatility {

class FbmGenerator {
public:
    FbmGenerator(double hurst, int num_steps, double T);
    std::vector<double> generate_path(std::mt19937& rng);

private:
    void precompute_eigenvalues();
    double compute_autocovariance(int k) const;

    double m_hurst;
    int m_num_steps;
    double m_T;
    double m_dt = m_T / (double) m_num_steps;
    double m_dt_squared = std::powf(m_dt, 2*m_hurst);
    std::vector<double> m_eigenvalues;
};

} // namespace RoughVolatility

#endif // FBM_GENERATOR_H
