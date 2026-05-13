#ifndef FBM_GENERATOR_H
#define FBM_GENERATOR_H

#include <vector>

namespace RoughVolatility {

class FbmGenerator {
public:
    FbmGenerator(double hurst, int num_steps, double T);
    std::vector<double> generate_path();

private:
    void precompute_eigenvalues();

    double m_hurst;
    int m_num_steps;
    double m_T;
    std::vector<double> m_eigenvalues;
};

} // namespace RoughVolatility

#endif // FBM_GENERATOR_H
