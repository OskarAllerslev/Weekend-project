# Monte Carlo Validation & Visualization
# Description: Analyzes terminal prices from the C++ Rough Volatility Pricer

file_path            <- "data/sim_prices.csv"
strike_price         <- 55.0
mean_reversion_level <- 50.0 

sim_data <- readr::read_csv(file = file_path, show_col_types = FALSE) |> 
  dplyr::as_tibble() |> 
  dplyr::mutate(
    payoff       = base::pmax(X - strike_price, 0),
    sim_id       = dplyr::row_number(),
    running_mean = base::cumsum(payoff) / sim_id
  )

num_sims    <- base::nrow(sim_data)
mc_price    <- base::mean(sim_data$payoff)
mc_sd       <- stats::sd(sim_data$payoff)
mc_se       <- mc_sd / base::sqrt(num_sims)

ci_lower    <- mc_price - 1.96 * mc_se
ci_upper    <- mc_price + 1.96 * mc_se

prob_itm    <- base::mean(sim_data$X > strike_price) * 100
cond_payoff <- base::mean(sim_data$payoff[sim_data$payoff > 0])

base::cat(base::sprintf("Expected Option Price: %.4f\n", mc_price))
base::cat(base::sprintf("Standard Error:        %.5f\n", mc_se))
base::cat(base::sprintf("95%% Confidence Int:    [%.4f, %.4f]\n", ci_lower, ci_upper))
base::cat(base::sprintf("Probability of ITM:    %.2f%%\n", prob_itm))
base::cat(base::sprintf("Expected Payoff (ITM): %.2f\n", cond_payoff))

plot_dist <- ggplot2::ggplot(sim_data, ggplot2::aes(x = X)) +
  ggplot2::geom_histogram(
    ggplot2::aes(y = ggplot2::after_stat(density)), 
    bins = 100, fill = "steelblue", color = "black", alpha = 0.8
  ) +
  ggplot2::geom_vline(xintercept = strike_price, color = "red", linetype = "dashed", linewidth = 1.2) +
  ggplot2::geom_vline(xintercept = mean_reversion_level, color = "green", linetype = "dotted", linewidth = 1) +
  ggplot2::annotate("text", x = strike_price + 1, y = 0.05, label = base::paste("Strike (K =", strike_price, ")"), color = "red", angle = 90) +
  ggplot2::annotate("text", x = mean_reversion_level - 1, y = 0.05, label = base::paste("Mean Reversion (Theta =", mean_reversion_level, ")"), color = "green", angle = 90) +
  ggplot2::theme_minimal() +
  ggplot2::labs(
    title    = "Distribution of Terminal Spot Prices (X_T)",
    subtitle = "Red dashed line indicates the In-The-Money (ITM) threshold",
    x        = "Terminal Spot Price (T=1)",
    y        = "Density"
  )

plot_conv <- ggplot2::ggplot(sim_data, ggplot2::aes(x = sim_id, y = running_mean)) +
  ggplot2::geom_line(color = "darkblue", linewidth = 0.8) +
  ggplot2::geom_hline(yintercept = mc_price, color = "red", linetype = "dashed") +
  ggplot2::theme_minimal() +
  ggplot2::labs(
    title    = "Monte Carlo Convergence",
    subtitle = base::sprintf("Converging towards expected price: %.4f", mc_price),
    x        = "Number of Simulations (M)",
    y        = "Running Expected Price"
  )


# ggplot2::ggsave("assets/terminal_distribution.png", plot = plot_dist, width = 8, height = 5, dpi = 300)
# ggplot2::ggsave("assets/mc_convergence.png", plot = plot_conv, width = 8, height = 5, dpi = 300)