#ifndef SDE_ENGINE_H
#define SDE_ENGINE_H

#include <vector>

namespace RoughVolatility {

class SdeEngine {
public:
    SdeEngine(double kappa, double theta, double v0, double nu, double x0);
    double simulate_path(const std::vector<double>& fbm_path, double T, int num_steps);

private:
    double m_kappa;
    double m_theta;
    double m_v0;
    double m_nu;
    double m_x0;
};

} // namespace RoughVolatility

#endif // SDE_ENGINE_H
