cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

# ---- Project ----
# Note: update this to your new project's name and version
project(
  project_euler
  VERSION 1.0
  LANGUAGES CXX
)

# ---- Include guards ----

# include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR
      "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
  )
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# ---- Add dependencies via CPM ----
# see https://github.com/TheLartians/CPM.cmake for more info

find_package(fmt REQUIRED)
# ---- Add source files ----

# Note: globbing sources is considered bad practice as CMake's generators may not detect new files
# automatically. Keep that in mind when changing files, or explicitly mention them here.

# ---- Create library ----
# add_executable(project_euler src/impls/tests.cpp src/impls/catch_main.cpp)

add_executable(project_euler 
# src/highly_divisible_triangular_number.cpp
# src/counting_sundays.cpp
# src/even_fibonacci.cpp
# src/combinatoric_selection.cpp
# src/largest_product_in_a_series.cpp
# src/largest_palindrom_product.cpp
# src/power_digit_sum.cpp
# src/square_remainder.cpp
src/square_root_convergents.cpp
# src/square_root_digital_expansion.cpp
# src/square_free_binomial.cpp
# src/reciprocal_cycles.cpp
# src/prime_permutation.cpp
# src/prime-k-factorial.cpp
# src/ordered_radical.cpp
# src/largest_prime_factor.cpp
# src/10001st_prime.cpp
# src/consecutive_prime_sum.cpp
# src/self_powers.cpp
# src/longest_collatz_sequence.cpp
# src/sum_of_primes.cpp
# src/largest_exponential.cpp
# src/special_pythagorian_triplet.cpp
# src/triangle_containment.cpp
)

# Note: for header-only libraries change all PUBLIC flags to INTERFACE and create an interface
# target: add_library(project_euler INTERFACE) set_target_properties(project_euler PROPERTIES
# INTERFACE_COMPILE_FEATURES cxx_std_17)

target_compile_features(project_euler PUBLIC cxx_std_20)
target_compile_options(project_euler PRIVATE -ggdb -fno-omit-frame-pointer  -fsanitize=address -fsanitize=undefined -pedantic -Wall -Wextra -Wcast-align -Wcast-qual -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 -Winit-self -Wlogical-op -Wmissing-declarations -Wmissing-include-dirs -Wnoexcept -Wold-style-cast -Woverloaded-virtual -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=5 -Wswitch-default -Wundef -Wno-unused )

target_include_directories(
	project_euler PUBLIC $<BUILD_INTERFACE: ${fmt_INCLUDE_DIRS}  >
)

target_link_libraries(project_euler PRIVATE -lasan -ggdb -lubsan fmt::fmt  )

# ---- Create an installable target ----
# this allows users to install and find the library via `find_package()`.

# the location where the project's version header will be placed should match the project's regular
# header paths

