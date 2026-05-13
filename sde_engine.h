#ifndef SDE_ENGINE_H
#define SDE_ENGINE_H

#include <vector>

namespace RoughVolatility {

class SdeEngine {
public:
    SdeEngine(double kappa, double theta, double v0, double nu, double x0, double hurst, double T, int num_steps);
    double simulate_path(const std::vector<double>& fbm_path);

private:
    double m_kappa;
    double m_theta;
    double m_v0;
    double m_nu;
    double m_x0;
    double m_hurst;
    double m_T;
    int m_num_steps;
};

} // namespace RoughVolatility

#endif // SDE_ENGINE_H
