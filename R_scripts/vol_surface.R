

data <- readr::read_csv(
  file = "data/surface_prices.csv"
)  |> 
  dplyr::as_tibble()


spot_price <- 50L
risk_free = 0.0


bs_call_price <- function(S, K, T, sigma, r) {
  d1 <- (base::log(S / K) + (r + 0.5 * sigma^2) * T) / (sigma * base::sqrt(T))
  d2 <- d1 - sigma * base::sqrt(T)
  
  price <- S * stats::pnorm(d1) - K * base::exp(-r * T) * stats::pnorm(d2)
  return(price)
}
calc_implied_vol <- function(target_price, S, K, T, r) {
  intrinsic_value <- base::max(S - K, 0)
  if (target_price <= intrinsic_value) {
    return(NA_real_)
  }
  
  obj_fun <- function(sigma) {
    bs_call_price(S, K, T, sigma, r) - target_price
  }
  
  res <- base::tryCatch({
    stats::uniroot(obj_fun, interval = c(0.001, 5.0), tol = 1e-6)$root
  }, error = function(e) {
    return(NA_real_)
  })
  
  return(res)
}

vol_data <- data  |> 
  dplyr::rowwise() |> 
  dplyr::mutate(
    implied_vol = calc_implied_vol(
      target_price = Price, 
      S = spot_price, 
      K = K, 
      T = T, 
      r = risk_free
    )
  ) |> 
  dplyr::ungroup() |> 
  dplyr::filter(!base::is.na(implied_vol)) |> 
  dplyr::mutate(T_factor = base::as.factor(T))

plot_smile <- ggplot2::ggplot(vol_data, ggplot2::aes(x = K, y = implied_vol, color = T_factor, group = T_factor)) +
  ggplot2::geom_line(linewidth = 1.2) +
  ggplot2::geom_point(size = 3) +
  ggplot2::scale_y_continuous(labels = scales::percent_format(accuracy = 1)) +
  ggplot2::scale_color_viridis_d(name = "Maturity (T)", option = "plasma") +
  ggplot2::theme_minimal() +
  ggplot2::theme(
    legend.position = "right",
    panel.grid.minor = ggplot2::element_blank()
  ) +
  ggplot2::labs(
    title    = "Rough Volatility Surface: Implied Volatility Smiles",
    subtitle = "Exhibiting pronounced short-term skew typical for H = 0.1",
    x        = "Strike Price (K)",
    y        = "Implied Volatility (IV)"
  )

# Display the plot
base::print(plot_smile)

# ggplot2::ggsave("assets/volatility_smile.png", plot = plot_smile, width = 8, height = 5, dpi = 300)
