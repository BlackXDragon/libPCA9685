# PCA9685 C/C++ library
cmake_minimum_required(VERSION 3.5)

project(PCA9685)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dependencies
find_library(I2C_LIB i2c)

# Library
add_library(PCA9685 SHARED src/PCA9685.c)
target_include_directories(PCA9685 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(PCA9685 ${I2C_LIB})

# Install
install(TARGETS PCA9685 DESTINATION lib)
install(FILES include/PCA9685/PCA9685.h DESTINATION PCA9685/include)

# Test executable
add_executable(testPCA9685 test/testPCA9685.c)
target_link_libraries(testPCA9685 PCA9685)
