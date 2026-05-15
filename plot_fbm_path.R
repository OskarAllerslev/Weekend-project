


fbm_data <- readr::read_csv(
  file = "fbm_path"
)  |> 
  dplyr::as_tibble()


ggplot2::ggplot( data = fbm_data, mapping = ggplot2::aes(x = step, y = value)) +
  ggplot2::geom_line(linewidth = 0.1)


