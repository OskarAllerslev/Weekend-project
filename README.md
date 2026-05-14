# Rough Volatility Monte Carlo Pricer

## Project Objective
The goal of this project is to build a high-performance, pure C++ Monte Carlo pricing engine for derivatives in energy markets. We model the underlying asset (e.g., electricity spot price) using a mean-reverting Stochastic Differential Equation (SDE) coupled with a Rough Bergomi-inspired volatility process driven by a Fractional Brownian Motion (fBm). 

The primary computational challenge is simulating the fBm efficiently. Instead of relying on the computationally expensive Cholesky decomposition $O(N^3)$, this engine implements the **Davies-Harte (Circulant Embedding) algorithm** via Fast Fourier Transform (FFT) to achieve exact simulation in $O(N \log N)$ time.

---

## Mathematical Framework

We consider an underlying asset following these dynamics:

$$dX_t = \kappa(\theta - X_t) dt + \sqrt{V_t} X_t dW_t^{(1)}$$

Where:
* $\kappa$ is the mean reversion speed.
* $\theta$ is the long-term mean level.
* $W_t^{(1)}$ is a standard Wiener process.

The process $V_t$ represents the stochastic volatility. To capture the "roughness" observed in energy markets without introducing a full secondary SDE, we define the variance explicitly:

$$V_t = V_0 \exp\left(\nu W_t^H - \frac{1}{2} \nu^2 t^{2H}\right), \quad H \in (0, 0.5)$$

To simulate the fractional Brownian motion $W_t^H$, we rely on its covariance function:

$$\text{COV}(W_t^H, W_s^H) = \frac{1}{2}\left(t^{2H} + s^{2H} - |t-s|^{2H}\right)$$

---

## Computational Strategy

### 1. Simulating Fractional Brownian Motion (The Davies-Harte Method)
Because the covariance matrix of fBm implies $O(N^3)$ complexity using Cholesky decomposition, we simulate the *increments* of the fBm (fractional Gaussian noise, fGn) instead. fGn is a stationary Gaussian process.

The autocovariance function of fGn over a time step $\Delta t$ is:

$$\gamma(k) = \frac{1}{2} \Delta t^{2H} \left(|k+1|^{2H} - 2|k|^{2H} + |k-1|^{2H}\right)$$

**The Davies-Harte Algorithm:**
1. Compute the autocovariance sequence $\gamma(k)$ for $k = 0, 1, \dots, N$.
2. Embed this sequence into a symmetric circulant matrix of size $2N$.
3. Compute the eigenvalues of this circulant matrix using the Discrete Fourier Transform (FFT).
4. Generate a sequence of complex standard normal random variables.
5. Multiply the random variables by the square root of the eigenvalues.
6. Apply the Inverse Fast Fourier Transform (IFFT).
7. Extract the first $N$ elements and take the real part to get the exact fGn increments.
8. Compute the cumulative sum of the increments to obtain the fBm path $W_t^H$.

### 2. Simulating the SDE (Euler-Maruyama Discretization)
With the fBm path $W_t^H$ pre-computed for a given trajectory, we discretize the main SDE using the Euler-Maruyama scheme over a time grid $t_i = i \Delta t$:

$$X_{i+1} = X_i + \kappa(\theta - X_i)\Delta t + \sqrt{V_i} X_i \sqrt{\Delta t} Z_{i+1}$$

Where:
* $Z_{i+1} \sim \mathcal{N}(0,1)$ is a standard normal random draw.
* $V_i = V_0 \exp\left(\nu W_{t_i}^H - \frac{1}{2} \nu^2 t_i^{2H}\right)$.

---

## Algorithm / Pseudocode

```text
Algorithm: Monte Carlo Pricing with Rough Volatility

Input: N_sims, N_steps, T, H, kappa, theta, V0, nu, X0
Output: Expected Payoff

dt = T / N_steps
Initialize total_payoff = 0.0

// Step 1: Pre-compute Davies-Harte Eigenvalues (Done once)
gamma = Compute_fGn_Autocovariance(N_steps, dt, H)
eigenvalues = FFT(Circulant_Embed(gamma))

// Step 2: Monte Carlo Loop
Parallel For sim = 1 to N_sims:
    // Generate fBm path exactly
    complex_normals = Generate_Complex_Normals(2 * N_steps)
    fGn_increments = IFFT(sqrt(eigenvalues) * complex_normals)
    W_H = Cumulative_Sum(Real_Part(fGn_increments)[0 to N_steps-1])
    
    // Euler-Maruyama for SDE
    X_t = X0
    For i = 0 to N_steps - 1:
        t = i * dt
        V_t = V0 * exp(nu * W_H[i] - 0.5 * nu^2 * t^(2H))
        Z = Draw_Standard_Normal()
        X_t = X_t + kappa * (theta - X_t) * dt + sqrt(V_t) * X_t * sqrt(dt) * Z
    
    // Calculate payoff (e.g., European Call with Strike K)
    Payoff = max(X_t - K, 0)
    total_payoff += Payoff

// Step 3: Result
Return (total_payoff / N_sims) * Discount_Factor
```

---

## Literature & Theoretical References

To understand the theoretical foundation of the exact simulation methods and rough volatility, refer to the following papers:

1. **The Exact Simulation Method:**
   * Davies, R. B., & Harte, D. S. (1987). *Tests for Hurst effect*. Biometrika, 74(1), 95-101.
   * Wood, A. T., & Chan, G. (1994). *Simulation of stationary Gaussian processes in $[0, 1]^d$*. Journal of computational and graphical statistics, 3(4), 409-432.

2. **Rough Volatility Framework:**
   * Gatheral, J., Jaisson, T., & Rosenbaum, M. (2018). *Volatility is rough*. Quantitative Finance, 18(6), 933-949.
   * Bayer, C., Friz, P., & Gatheral, J. (2016). *Pricing under rough volatility*. Quantitative Finance, 16(6), 887-904.





-----------------------------------------

# Findings and results

Here is a simulated fbm path with $H = 0.01$. 
<img src="assets/fbm_plot.png" alt="Rough Volatility Path Simulation" width="600"/>
