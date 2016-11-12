cmake_minimum_required(VERSION 3.6)
project(ProjetDTW)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

set(SOURCE_FILES main.cpp dtw.h dtw.h dtw.cpp FFTReal.cpp FFTReal.h libmfccOptim.cpp libmfccOptim.h WavToMfcc.cpp WavToMfcc.h)
add_executable(ProjetDTW ${SOURCE_FILES})