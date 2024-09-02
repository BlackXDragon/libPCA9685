# PCA9685 C/C++ library
cmake_minimum_required(VERSION 3.5)

project(PCA9685)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Dependencies
find_library(I2C_LIB i2c)

# Library
add_library(PCA9685 SHARED src/PCA9685.c)
target_include_directories(PCA9685 INTERFACE include)
target_link_libraries(PCA9685 ${I2C_LIB})

# Install
install(
	TARGETS PCA9685
	EXPORT PCA9685
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
)
install(
	EXPORT PCA9685
	DESTINATION share/PCA9685/cmake
	FILE PCA9685Config.cmake
)

# Test executable
add_executable(testPCA9685 test/testPCA9685.c)
target_link_libraries(testPCA9685 PCA9685)
