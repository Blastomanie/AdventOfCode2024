cmake_minimum_required(VERSION 3.5.0)

project(utils)

add_library(utils INTERFACE)

target_include_directories(utils INTERFACE .)

target_compile_features(utils INTERFACE cxx_std_17)