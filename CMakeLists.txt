cmake_minimum_required(VERSION 3.10)

project(Weekend-project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# include_directories(eigen)

add_executable(exec_program 
    main.cpp
    fbm_generator.cpp
    sde_engine.cpp
    payoff.cpp
    monte_carlo_pricer.cpp
)
