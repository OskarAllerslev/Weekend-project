#ifndef PAYOFF_H
#define PAYOFF_H

namespace RoughVolatility {

class EuropeanCallPayoff {
public:
    explicit EuropeanCallPayoff(double strike);
    double operator()(double spot) const;

private:
    double m_strike;
};

} // namespace RoughVolatility

#endif // PAYOFF_H
