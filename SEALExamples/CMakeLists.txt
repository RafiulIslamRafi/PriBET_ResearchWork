cmake_minimum_required(VERSION 3.10)

project(SEALExamples VERSION 3.0.0 LANGUAGES CXX)

# Executable will be in ../bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/../bin)

add_executable(CompareByHammingDistance CompareTwoNumber_PETprotocol_hamming_distance.cpp)
add_executable(Project_file Project.cpp)
add_executable(CompareByHammingDistanceMultiple CompareMultipleWithOne_PriBETprotocol_hamming_distance.cpp)
add_executable(privacy_preserving_plagiarism privacy_preserving_plagiarism.cpp)
add_executable(rafi_binary_file rafi.cpp)
add_executable(sealexamples main.cpp)

# Import SEAL
find_package(SEAL 3.0.0 EXACT REQUIRED)


# Link SEAL
target_link_libraries(CompareByHammingDistance SEAL::seal)
target_link_libraries(CompareByHammingDistanceMultiple SEAL::seal)
target_link_libraries(Project_file SEAL::seal)
target_link_libraries(privacy_preserving_plagiarism SEAL::seal)
target_link_libraries(rafi_binary_file SEAL::seal)
target_link_libraries(sealexamples SEAL::seal)
