# Weekend-project




# Mathematical framework
We consider an underlying asset following these dynamics

$$
dX_t = \kappa ( \theta - X_t) dt + \sqrt{V_t} X_t dW_t^{(1)}
$$

Where $\kappa$ is the mean reversion speed. 
$\theta$ is the long term mean level. 
$W_t^{(1)}$ is a standard Wiener process.
The process $V_t$ should represent the volatility, and for simplicity we set 
$$
V_t = V_0 e^{\nu W_t^H - 1/2 \nu^2 t^{2H}} \quad H \in (0,0.5)
$$

To simulate the fractional brownian motion we rely on the covariance function 
$$
\text{COV}(W_t^H, W_s^H) = \frac{1}{2}\left( t^{2H} + s^{2H} - |t-s|^{2H} \right)
$$


# Computational considerations

We apply the Cholesky decomposition for fast simulations.
