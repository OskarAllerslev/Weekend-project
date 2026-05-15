


data <- readr::read_csv(
  file = "data/sim_prices.csv"
)  |> 
  dplyr::as_tibble()

strike <- 55 

dat <- data  |> 
  dplyr::mutate(
    payoff = base::pmax(X - strike, 0)
  )  


mc_price <- mean(dat$payoff)
mc_sd <- sd(dat$payoff)
mc_se <- mc_sd / sqrt(nrow(data))

ci_lower <- mc_price - 1.96 * mc_se
ci_upper <- mc_price + 1.96 * mc_se

cat(sprintf("Forventet Pris: %.4f\n", mc_price))
cat(sprintf("Standard Error: %.5f\n", mc_se))
cat(sprintf("95%% Konfidensinterval: [%.4f, %.4f]\n", ci_lower, ci_upper))




prob_itm <- mean(dat$X > strike) * 100
cond_payoff <- mean(dat$payoff[dat$payoff > 0])

cat(sprintf("Sandsynlighed for ITM: %.2f%%\n", prob_itm))
cat(sprintf("Gennemsnitlig udbetaling NÅR vi er ITM: %.2f\n", cond_payoff))


library(ggplot2)

ggplot(dat, aes(x = X)) +
  geom_histogram(aes(y = ..density..), bins = 100, fill = "steelblue", color = "black", alpha = 0.8) +
  geom_vline(xintercept = strike, color = "red", linetype = "dashed", size = 1.2) +
  geom_vline(xintercept = 50, color = "green", linetype = "dotted", size = 1) +
  annotate("text", x = strike + 1, y = 0.05, label = paste("Strike (K =", strike, ")"), color = "red", angle = 90) +
  annotate("text", x = 49, y = 0.05, label = expression("Mean-reversion level (" * theta * " = 50)"), color = "green", angle = 90) +
  theme_minimal() +
  labs(title = "Fordeling af Terminale Elpriser (X_T)",
       subtitle = "Rød linje markerer grænsen for udbetaling (In-The-Money)",
       x = "Spotpris ved Udløb (T=1)",
       y = "Tæthed (Density)")



# Beregn det løbende gennemsnit af prisen
dat <- dat |> 
  dplyr::mutate(
    sim_nummer = dplyr::row_number(),
    running_mean = cumsum(payoff) / sim_nummer
  )

# Plot konvergensen
ggplot(dat, aes(x = sim_nummer, y = running_mean)) +
  geom_line(color = "darkblue", size = 0.8) +
  geom_hline(yintercept = mc_price, color = "red", linetype = "dashed") +
  theme_minimal() +
  labs(
    title = "Monte Carlo Konvergens af Optionspris",
    subtitle = sprintf("Stabiliserer sig omkring forventet pris: %.4f", mc_price),
    x = "Antal Simuleringer (M)",
    y = "Løbende Forventet Pris"
  )